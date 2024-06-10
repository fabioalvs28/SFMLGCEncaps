#pragma once

#include "../Core/framework.h"

class GameObject;
class Scene;

class SceneManager
{
	friend class Scene;
	friend class GameManager;

public : 

	SceneManager() {};
	virtual ~SceneManager() {};

	void CreateScene(); 
	void RenderScene(); 


private : 

	void Update();
	GCLinkedList<Scene*> m_sceneList; 
	GCLinkedList<Scene*> m_loadedSceneList;

};



class Scene {

	friend class SceneManager;

public :
	Scene() {}
	Scene( GCLinkedList<Scene*>& m_loadedSceneList ) ;
	virtual ~Scene() {};


	void UnloadScene();
	void DestroyScene();
	void LoadScene() ;
	GameObject* CreateGameObject( const char* name/* = GameObject */, bool active/* = true */ );

private : 
	static GCLinkedList<Scene*>& m_loadedSceneList;
	void Update();
	bool m_isActive; 
	GCLinkedList<GameObject*> m_gameObjectsList;
	GCLinkedListNode<Scene*>* m_loadedNode;
	GCLinkedListNode<Scene*>* m_allSceneNode;

};
