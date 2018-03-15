#include "pch.h"
#include "GameScene.h"
#include "RenderData.h"
#include "GameStateData.h"
#include "FinalDestination.h"
#include "CharacterController.h"
#include "Player.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	for (int i = 0; i < 4; i++)
	{
		if (entities[i])
		{
			delete entities[i];
			entities[i] = nullptr;
		}
	}
}

void GameScene::Initialise(RenderData * _RD,
	GameStateData* _GSD, int _outputWidth,
	int _outputHeight, std::unique_ptr<DirectX::AudioEngine>& _audEngine)
{
	m_RD = _RD;
	m_GSD = _GSD;

	c_manager.PopulateCharacterList(_RD);

	//GEP::This is where I am creating the test objects
	m_cam = new Camera(static_cast<float>(_outputWidth), static_cast<float>(_outputHeight), 1.0f, 1000.0f);
	m_RD->m_cam = m_cam;
	m_3DObjects.push_back(m_cam);

	//creating a stage
	//could pass the name of the stage as a function paratemter
	game_stage = std::make_unique<FinalDestination>();
	game_stage->init(m_RD,m_GSD);


	TestPBGO3D* test3d = new TestPBGO3D();
	test3d->SetScale(5.0f);
	test3d->Init();
	m_3DObjects.push_back(test3d);

	for (int i = 0; i < 2; i++)
	{
		entities[i] = new Player(i);
		players[i] = new Character(c_manager.GetCharacterByName("Character001"));
		players[i]->SetSpawn(Vector2(i * 200 + 400, 100));

		players[i]->CreatePhysics(_RD);
		players[i]->GetPhysics()->SetDrag(0.5f);
		players[i]->GetPhysics()->SetBounce(0.4f);

		float width = players[i]->TextureSize().x;
		float height = players[i]->TextureSize().y;
		Rectangle rect = Rectangle
		(players[i]->GetPos().x, players[i]->GetPos().y, width, height);
		players[i]->GetPhysics()->SetCollider(rect);

		m_2DObjects.push_back(players[i]);
		m_GSD->objects_in_scene.push_back(players[i]->GetPhysics());
		entities[i]->SetCharacter(players[i]);
	}

	for (int i = 0; i < m_2DObjects.size(); i++)
	{
		for (int j = 0; j < listeners.size(); j++)
		{
			m_2DObjects[i]->addListener(listeners[j]);
		}
	}

	game_stage->addObjectsToScene(m_2DObjects);
	
}

void GameScene::Update(DX::StepTimer const & timer, std::unique_ptr<DirectX::AudioEngine>& _audEngine)
{
	Scene::Update(timer, _audEngine);
	game_stage->update(m_GSD);

	if (m_GSD->game_actions[0].size() > 0)
	{
		//code for testing zoom
		//if (m_GSD->game_actions[0][0] == GameAction::P_RELEASE_SPECIAL)
		//{
		//	for (int i = 0; i < m_2DObjects.size(); i++)
		//	{
		//		ImageGO2D* temp = static_cast<ImageGO2D*>(m_2DObjects[i]);
		//		temp->scaleFromPoint(Vector2(0, 0), Vector2(temp->GetScale().x + 0.1f, temp->GetScale().y + 0.1f));
		//	}
		//}
		//if (m_GSD->game_actions[0][0] == GameAction::P_RELEASE_BASIC)
		//{
		//	for (int i = 0; i < m_2DObjects.size(); i++)
		//	{
		//		ImageGO2D* temp = static_cast<ImageGO2D*>(m_2DObjects[i]);
		//		temp->scaleFromPoint(Vector2(0, 0), Vector2(temp->GetScale().x - 0.1f, temp->GetScale().y - 0.1f));
		//	}
		//}
	}

	for (int i = 0; i < m_2DObjects.size(); i++)
	{
		ImageGO2D* temp = static_cast<ImageGO2D*>(m_2DObjects[i]);
		//temp->scaleFromPoint(Vector2(800, 600), Vector2(temp->GetScale().x + 0.1, temp->GetScale().y + 0.1));
	}
}

void GameScene::Reset()
{
	for (int i = 0; i < m_GSD->objects_in_scene.size(); i++)
	{
		m_GSD->objects_in_scene[i]->ResetForce(BOTH_AXES);
	}
	for (int i = 0; i < m_2DObjects.size(); i++)
	{
		m_2DObjects[i]->ResetPos();
	}

	for (int i = 0; i < m_3DObjects.size(); i++)
	{
		m_3DObjects[i]->ResetPos();
	}
}