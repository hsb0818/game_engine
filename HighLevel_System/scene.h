#pragma once

#define SCENE_CREATE(_SCENE_BASED_CLASS_, _SCENE_NAME_)	{ R_SCENE_MNG().Insert(new _SCENE_BASED_CLASS_(), _SCENE_NAME_); }
#define SCENE_DELETE(_SCENE_NAME_)	{ R_SCENE_MNG().Delete(_SCENE_NAME_); }
#define SCENE_START(_SCENE_NAME_) { R_SCENE_MNG().StartScene(_SCENE_NAME_); }
#define SCENE_CHANGE(_SCENE_NAME_)	{ R_SCENE_MNG().ChangeScene(_SCENE_NAME_); }
//	#define SCENE_FRAME_RENDER	{ R_SCENE_MNG().FrameRender(); }
//	#define SCENE_FRAME_MOVE	{ R_SCENE_MNG().FrameRender(); }

//HL is highlevel.
namespace SOC_System {
	using namespace SOC_Memory;

	class SceneBase : public BaseMP
	{
	private:
		SceneBase(const SceneBase& rhs);

	public:
		SceneBase() {}
		virtual ~SceneBase() {}

		virtual void Init() = 0;
		virtual void Destroy() = 0;
		virtual void FrameMove(double elapsedTime) = 0;
		virtual void FrameRender(double elapsedTime) = 0;
		virtual void Event(MSG* pMsg) = 0;
	};

	class SceneHelper : public BaseMP
	{
	public:
		enum SceneType
		{
			NORMAL = 0,
			OVERLAP
		};

	public:
		SceneHelper(SceneBase* pSceneBase, const wchar_t* pSceneName = L"None", SceneType type = NORMAL)
			: m_pScene(pSceneBase)
			, m_pSceneName(pSceneName)
			, m_type(type)
			, m_on(false)
			, m_pRenderTimer(nullptr)
			, m_pMoveTimer(nullptr)			
		{
			m_pMoveTimer = new MPTimer();
			m_pRenderTimer = new MPTimer();

			if (EnvTest() == false)
				return;

			m_pMoveTimer->Start();
			m_pRenderTimer->Start();
		}
		~SceneHelper() 
		{
			Delete();
		}

	private:
		void Delete()
		{
			SAFE_DELETE(m_pRenderTimer);
			SAFE_DELETE(m_pMoveTimer);
			SAFE_DELETE(m_pScene);
		}

	public:
		void On() { m_on = true; }
		void Off() { m_on = false; }
		bool IsOn() { return m_on; }
		void SetType(SceneType type) { m_type = type; }

		void Clear()
		{
			if (EnvTest() == false)
				return;

			m_pScene->Destroy();
			m_on = true;
			m_pMoveTimer->Start();
			m_pRenderTimer->Start();
		}

		void FrameRender()
		{
			if (m_on == false)
				return;

			m_pRenderTimer->End();
			m_pScene->FrameRender(m_pRenderTimer->CalcRunTime());
			m_pRenderTimer->StartisEnd();
		}

		void FrameMove()
		{
			if (m_on == false)
				return;

			m_pMoveTimer->End();
			m_pScene->FrameMove(m_pMoveTimer->CalcRunTime());
			m_pMoveTimer->StartisEnd();
		}
		
		bool EnvTest()
		{
			if (m_pScene == nullptr)
				return false;
			else if (m_pMoveTimer == nullptr)
				return false;
			else if (m_pRenderTimer == nullptr)
				return false;

			return true;
		}

		SceneBase* Get() { return m_pScene; }
		SceneType Type() { return m_type; }
		const wchar_t* Name() { return m_pSceneName; }

	private:
		bool m_on;
		SceneType m_type;
		SceneBase* m_pScene;
		const wchar_t* m_pSceneName;
		MPTimer* m_pMoveTimer;
		MPTimer* m_pRenderTimer;
	};

	class SceneManager : public Singleton<SceneManager>
	{
	private:
		typedef std::vector<SceneHelper*> SCENEVECTOR;
	public:
		SceneManager() : m_pCurScene(nullptr) 
		{}
		~SceneManager() { DeleteAll(); }

	public:
		void Insert(SceneBase* pNewScene, const wchar_t* pSceneName = L"None")
		{
			if (pNewScene == nullptr)
				return;

			SceneHelper* pSceneHelper = new SceneHelper(pNewScene, pSceneName);
			if (pSceneHelper == nullptr)
				return;

			m_vSceneList.push_back(pSceneHelper);
		}

		void Delete(wchar_t* pSceneName)
		{
			SCENEVECTOR::iterator itr;
			for (itr = m_vSceneList.begin(); itr != m_vSceneList.end(); itr++)
			{
				if (lstrcmp((*itr)->Name(), pSceneName) != 0)
					continue;

				SAFE_DELETE(*itr);
				m_vSceneList.erase(itr);
				break;
			}
		}

		void DeleteAll()
		{
			SCENEVECTOR::iterator itr;
			for (itr = m_vSceneList.begin(); itr != m_vSceneList.end(); itr++)
			{
				SAFE_DELETE(*itr);
			}

			m_vSceneList.clear();
			SCENEVECTOR(m_vSceneList).swap(m_vSceneList);
		}

		void FrameRender()
		{
			if (m_pCurScene)
				m_pCurScene->FrameRender();

			SCENEVECTOR::iterator itr;
			for (itr = m_vSceneList.begin(); itr != m_vSceneList.end(); itr++)
			{
				if (m_pCurScene == (*itr))
					continue;
				if ((*itr)->Type() == SceneHelper::OVERLAP)
					(*itr)->FrameRender();
			}
		}

		void FrameMove()
		{
			if (m_pCurScene)
				m_pCurScene->FrameMove();

			SCENEVECTOR::iterator itr;
			for (itr = m_vSceneList.begin(); itr != m_vSceneList.end(); itr++)
			{
				if (m_pCurScene == (*itr))
					continue;
				if ((*itr)->Type() == SceneHelper::OVERLAP)
					(*itr)->FrameMove();
			}
		}

		void Event(MSG* pMsg)
		{
			if (m_pCurScene)
				m_pCurScene->Get()->Event(pMsg);
		}

		void StartScene(wchar_t* pSceneName)
		{
			ChangeScene(pSceneName);
		}

		SceneHelper* ChangeScene(wchar_t* pSceneName)
		{
			if (m_pCurScene)
			{
				if (lstrcmp(pSceneName, m_pCurScene->Name()) == 0)
					return m_pCurScene;
			}

			SCENEVECTOR::iterator itr;
			for (itr = m_vSceneList.begin(); itr != m_vSceneList.end(); itr++)
			{
				if (lstrcmp(pSceneName, (*itr)->Name()) != 0)
					continue;

				if (m_pCurScene)
				{
					m_pCurScene->Clear();
					m_pCurScene->Off();
				}

				m_pCurScene = (*itr);
				m_pCurScene->Get()->Init();
				m_pCurScene->On();

				return m_pCurScene;
			}

			return nullptr;
		}

		SceneHelper* GetCurrentScene() { return m_pCurScene; }
		SceneHelper* GetScene(wchar_t* pSceneName)
		{
			SCENEVECTOR::iterator itr;
			for (itr = m_vSceneList.begin(); itr != m_vSceneList.end(); itr++)
			{
				if (lstrcmp(pSceneName, (*itr)->Name()) != 0)
					continue;

				return m_pCurScene;
			}

			return nullptr;
		}

		bool EnvTest()
		{
			SCENEVECTOR::iterator itr;
			for (itr = m_vSceneList.begin(); itr != m_vSceneList.end(); itr++)
			{
				if ((*itr)->EnvTest() == false)
					return false;
			}

			return true;
		}

	private:
		SceneHelper* m_pCurScene;
		SCENEVECTOR m_vSceneList;
	};
}