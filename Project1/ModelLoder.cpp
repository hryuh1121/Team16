#include "ModelLoder.h"

void ModelLoder::LoadModel(const std::string& modelname, Model* model)
{
	modeldata[modelname] = model->CreateFromOBJ(0,modelname);
}

void ModelLoder::FindModel(const std::string& modelname,Model* model)
{
	std::map<std::string, Model*>::iterator it;
	it = modeldata.find(modelname);
	model = it->second;
}
