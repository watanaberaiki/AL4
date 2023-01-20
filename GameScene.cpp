#include "GameScene.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete spriteBG;
	delete object3d;
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->input = input;

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	//// テクスチャ読み込み
	//Sprite::LoadTexture(1, L"Resources/background.png");

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	//モデル読み込み
	//立方体
	XMFLOAT3 minModel = {}, maxModel = {};
	Model* model = Model::LoadFromObj("cone", minModel, maxModel);
	minModel = model->GetminModel();
	maxModel = model->GetmaxModel();
	//円柱
	XMFLOAT3 minModel2 = {}, maxModel2 = {};
	Model* model2 = Model::LoadFromObj("cylinder", minModel2, maxModel2);
	minModel2 = model2->GetminModel();
	maxModel2 = model2->GetmaxModel();

	//円柱
	cylinder = Object3d::Create();
	cylinder->SetModel(model2);
	cylinder->SetPosition({ 5.0, 0, 0 });

	//立方体
	cube = Object3d::Create();
	//オブジェクトにモデルを紐づける
	cube->SetModel(model);
}

void GameScene::Update()
{
	// 円柱移動
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		// 現在の座標を取得
		XMFLOAT3 position = cylinder->GetPosition();

		// 移動後の座標を計算
		if (input->PushKey(DIK_UP)) { position.z += 0.5f; }
		else if (input->PushKey(DIK_DOWN)) { position.z -= 0.5f; }
		if (input->PushKey(DIK_RIGHT)) { position.x += 0.5f; }
		else if (input->PushKey(DIK_LEFT)) { position.x -= 0.5f; }

		// 座標の変更を反映
		cylinder->SetPosition(position);
	}

	// 立方体移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		// 現在の座標を取得
		XMFLOAT3 position = cube->GetPosition();

		// 移動後の座標を計算
		if (input->PushKey(DIK_W)) { position.z += 0.5f; }
		else if (input->PushKey(DIK_S)) { position.z -= 0.5f; }
		if (input->PushKey(DIK_D)) { position.x += 0.5f; }
		else if (input->PushKey(DIK_A)) { position.x -= 0.5f; }

		// 座標の変更を反映
		cube->SetPosition(position);
	}
	cylinder->Update();

	cube->Update();

	
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	cylinder->Draw();

	cube->Draw();
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
