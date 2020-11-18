#pragma once
#include "Model.h"
#include <map>


class ModelLoder {

public:
	void LoadModel(const std::string& modelname,Model* model);

	void FindModel(const std::string& modelname,Model* model);
private:
	//���f���f�[�^���i�[���Ă���
	std::map<std::string,Model*> modeldata;
	//Model *model = nullptr;
};