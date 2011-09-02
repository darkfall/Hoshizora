//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#include "../include/JGE.h"
#include "../include/JRenderer.h"
#include "../include/JSoundSystem.h"
#include "../include/JResourceManager.h"
#include "../include/JFileSystem.h"

#include "tinyxml/tinyxml.h"

JResourceManager::JResourceManager()
{
	//mResourceRoot = "Res/";				// default root folder 

	mTextureList.clear();
	mTextureList.reserve(16);
	mTextureMap.clear();

	mQuadList.clear();
	mQuadList.reserve(128);
	mQuadMap.clear();

	mFontList.clear();
	mFontList.reserve(4);
	mFontMap.clear();

	mMusicList.clear();
	mMusicList.reserve(4);
	mMusicMap.clear();

	mSampleList.clear();
	mSampleList.reserve(8);
	mSampleMap.clear();

// 	mParticleEffectList.clear();
// 	mParticleEffectList.reserve(8);
// 	mParticleEffectMap.clear();
// 
// 	mMotionEmitterList.clear();
// 	mMotionEmitterList.reserve(16);
// 	mMotionEmitterMap.clear();
}


JResourceManager::~JResourceManager()
{
	
	RemoveAll();
}


void JResourceManager::RemoveAll()
{
	for (vector<JTexture *>::iterator tex = mTextureList.begin(); tex != mTextureList.end(); ++tex)
		delete *tex;

	mTextureList.clear();
	mTextureMap.clear();

	for (vector<JQuad *>::iterator quad = mQuadList.begin(); quad != mQuadList.end(); ++quad)
		delete *quad;

	mQuadList.clear();
	mQuadMap.clear();

	for (vector<JFont *>::iterator font = mFontList.begin(); font != mFontList.end(); ++font)
		delete *font;

	mFontList.clear();
	mFontMap.clear();

	for (vector<JMusic *>::iterator music = mMusicList.begin(); music != mMusicList.end(); ++music)
		delete *music;

	mMusicList.clear();
	mMusicMap.clear();

	for (vector<JSample *>::iterator sample = mSampleList.begin(); sample != mSampleList.end(); ++sample)
		delete *sample;

	mSampleList.clear();
	mSampleMap.clear();

// 	for (vector<JParticleEffect *>::iterator effect = mParticleEffectList.begin(); effect != mParticleEffectList.end(); ++effect)
// 		delete *effect;
// 
// 	mParticleEffectList.clear();
// 	mParticleEffectMap.clear();
// 
// 	for (vector<JMotionEmitter *>::iterator emitter = mMotionEmitterList.begin(); emitter != mMotionEmitterList.end(); ++emitter)
// 		delete *emitter;
// 
// 	mMotionEmitterList.clear();
// 	mMotionEmitterMap.clear();

}


bool JResourceManager::LoadResource(const string& resourceName)
{
	string path = /*mResourceRoot + */resourceName;

//	TiXmlDocument doc(path.c_str());
//	
//	if (!doc.LoadFile()) return false;
	
	JGE *engine = JGE::GetInstance();
	if (engine == NULL) return false;

	JFileSystem *fileSystem = JFileSystem::GetInstance();
	if (fileSystem == NULL) return false;

	if (!fileSystem->OpenFile(path.c_str())) return false;

	int size = fileSystem->GetFileSize();
	char *xmlBuffer = new char[size];
	fileSystem->ReadFile(xmlBuffer, size);

	TiXmlDocument doc;
	doc.Parse(xmlBuffer);

	TiXmlNode* resource = 0;
	TiXmlNode* node = 0;
	TiXmlElement* element = 0;

	resource = doc.FirstChild("resource"); 
	if (resource)
	{
		element = resource->ToElement();
		printf("---- Loading %s:%s\n", element->Value(), element->Attribute("name"));

		for (node = resource->FirstChild(); node; node = node->NextSibling())
		{
			element = node->ToElement();
			if (element != NULL)
			{
				if (strcmp(element->Value(), "texture")==0)
				{
					CreateTexture(element->Attribute("name"));
				}
				else if (strcmp(element->Value(), "quad")==0)
				{
					string quadName = element->Attribute("name");
					string textureName = element->Attribute("texture");
					float x = 0.0f;
					float y = 0.0f;
					float width = 16.0f;
					float height = 16.0f;
					float value;
					float hotspotX = 0.0f;
					float hotspotY = 0.0f;
					
					if (element->QueryFloatAttribute("x", &value) == TIXML_SUCCESS)
						x = value;
					
					if (element->QueryFloatAttribute("y", &value) == TIXML_SUCCESS)
						y = value;

					if (element->QueryFloatAttribute("width", &value) == TIXML_SUCCESS)
						width = value;

					if (element->QueryFloatAttribute("height", &value) == TIXML_SUCCESS)
						height = value;

					if (element->QueryFloatAttribute("w", &value) == TIXML_SUCCESS)
						width = value;

					if (element->QueryFloatAttribute("h", &value) == TIXML_SUCCESS)
						height = value;

					if (element->QueryFloatAttribute("hotspot.x", &value) == TIXML_SUCCESS)
						hotspotX = value;
					else
						hotspotX = width/2;

					if (element->QueryFloatAttribute("hotspot.y", &value) == TIXML_SUCCESS)
						hotspotY = value;
					else
						hotspotY = height/2;

// 					if (element->QueryFloatAttribute("regx", &value) == TIXML_SUCCESS)
// 						hotspotX = width/2;
// 
// 					if (element->QueryFloatAttribute("regy", &value) == TIXML_SUCCESS)
// 						hotspotY = height/2;
				
					int id = CreateQuad(quadName, textureName, x, y, width, height);
					if (id != INVALID_ID)
					{
						GetQuad(id)->SetHotSpot(hotspotX, hotspotY);
					}
				}
				else if (strcmp(element->Value(), "font")==0)
				{
				}
				else if (strcmp(element->Value(), "music")==0)
				{
					LoadMusic(element->Attribute("name"));
				}
				else if (strcmp(element->Value(), "sample")==0)
				{
					LoadSample(element->Attribute("name"));
				}
// 				else if (strcmp(element->Value(), "effect")==0)
// 				{
// 					RegisterParticleEffect(element->Attribute("name"));
// 				}
// 				else if (strcmp(element->Value(), "motion_emitter")==0)
// 				{
// 					RegisterMotionEmitter(element->Attribute("name"));
// 				}
			}
		}
		
	}
	
	fileSystem->CloseFile();
	delete[] xmlBuffer;
//	JGERelease();

	return true;
}


int JResourceManager::CreateTexture(const string &textureName)
{
	map<string, int>::iterator itr = mTextureMap.find(textureName);

	if (itr == mTextureMap.end())
	{
		string path = /*mResourceRoot + */textureName;

		printf("creating texture:%s\n", path.c_str());
		
		JTexture *tex = JRenderer::GetInstance()->LoadTexture(path.c_str());

		if (tex == NULL)
			return INVALID_ID;

		int id = mTextureList.size();
		mTextureList.push_back(tex);
		mTextureMap[textureName] = id;
		
		return id;
	}
	else
		return itr->second;
}


JTexture *JResourceManager::GetTexture(const string &textureName)
{
	map<string, int>::iterator itr = mTextureMap.find(textureName);

	if (itr == mTextureMap.end())
		return NULL;
	else
		return mTextureList[itr->second];
}


JTexture *JResourceManager::GetTexture(int id)
{
	if (id >=0 && id < (int)mTextureList.size())
		return mTextureList[id];
	else
		return NULL;
}


int JResourceManager::CreateQuad(const string &quadName, const string &textureName, float x, float y, float width, float height)
{
	map<string, int>::iterator itr = mQuadMap.find(quadName);

	if (itr == mQuadMap.end())
	{
		JTexture *tex = GetTexture(textureName);	
		if (tex == NULL)
		{
			int texId = CreateTexture(textureName);		// load texture if necessary
			tex = GetTexture(texId);
		}

		if (tex == NULL)								// no texture, no quad...
			return INVALID_ID;

		printf("creating quad:%s\n", quadName.c_str());

		int id = mQuadList.size();
		mQuadList.push_back(new JQuad(tex, x, y, width, height));

		mQuadMap[quadName] = id;
		
		return id;

	}
	else
		return itr->second;
}


JQuad *JResourceManager::GetQuad(const string &quadName)
{
	map<string, int>::iterator itr = mQuadMap.find(quadName);

	if (itr == mQuadMap.end())
		return NULL;
	else
		return mQuadList[itr->second];
}


JQuad *JResourceManager::GetQuad(int id)
{
	if (id >=0 && id < (int)mQuadList.size())
		return mQuadList[id];
	else
		return NULL;
}


int JResourceManager::LoadFont(const string &fontName)
{
	map<string, int>::iterator itr = mFontMap.find(fontName);

	if (itr == mFontMap.end())
	{
		string path = /*mResourceRoot + */fontName;

		printf("creating font:%s\n", path.c_str());
		
		int id = mFontList.size();
		///////////////////////////////////////
		mFontList.push_back(new JFont());	

		mFontMap[fontName] = id;
		
		return id;
	}
	else
		return itr->second;
}


JFont *JResourceManager::GetFont(const string &fontName)
{
	map<string, int>::iterator itr = mFontMap.find(fontName);

	if (itr == mFontMap.end())
		return NULL;
	else
		return mFontList[itr->second];
}


JFont *JResourceManager::GetFont(int id)
{
	if (id >=0 && id < (int)mFontList.size())
		return mFontList[id];
	else
		return NULL;
}


int JResourceManager::LoadMusic(const string &musicName)
{
	map<string, int>::iterator itr = mMusicMap.find(musicName);

	if (itr == mMusicMap.end())
	{
		string path = /*mResourceRoot + */musicName;

		printf("creating music:%s\n", path.c_str());
		
		JMusic *music = JSoundSystem::GetInstance()->LoadMusic(path.c_str());
		if (music == NULL)
			return INVALID_ID;

		int id = mMusicList.size();
		mMusicList.push_back(music);	

		mMusicMap[musicName] = id;
		
		return id;
	}
	else
		return itr->second;
}


JMusic *JResourceManager::GetMusic(const string &musicName)
{
	map<string, int>::iterator itr = mMusicMap.find(musicName);

	if (itr == mMusicMap.end())
		return NULL;
	else
		return mMusicList[itr->second];
}


JMusic *JResourceManager::GetMusic(int id)
{
	if (id >=0 && id < (int)mMusicList.size())
		return mMusicList[id];
	else
		return NULL;
}


int JResourceManager::LoadSample(const string &sampleName)
{
	map<string, int>::iterator itr = mSampleMap.find(sampleName);

	if (itr == mSampleMap.end())
	{
		string path = /*mResourceRoot + */sampleName;

		printf("creating sample:%s\n", path.c_str());
		
		JSample *sample = JSoundSystem::GetInstance()->LoadSample(path.c_str());
		if (sample == NULL)
			return INVALID_ID;

		int id = mSampleList.size();
		mSampleList.push_back(sample);	

		mSampleMap[sampleName] = id;
		
		return id;
	}
	else
		return itr->second;
}


JSample *JResourceManager::GetSample(const string &sampleName)
{
	map<string, int>::iterator itr = mSampleMap.find(sampleName);

	if (itr == mSampleMap.end())
		return NULL;
	else
		return mSampleList[itr->second];
}


JSample *JResourceManager::GetSample(int id)
{
	if (id >=0 && id < (int)mSampleList.size())
		return mSampleList[id];
	else
		return NULL;
}

// 
// 
// int JResourceManager::RegisterParticleEffect(const string &effectName)
// {
// 	map<string, int>::iterator itr = mParticleEffectMap.find(effectName);
// 
// 	if (itr == mParticleEffectMap.end())
// 	{
// 		string path = mResourceRoot + effectName;
// 		printf("creating effect:%s\n", path.c_str());
// 		JParticleEffect *effect = new JParticleEffect(path.c_str());
// 
// 		if (effect == NULL)
// 			return INVALID_ID;
// 
// 
// 		int id = mParticleEffectList.size();
// 		mParticleEffectList.push_back(effect);
// 
// 		mParticleEffectMap[effectName] = id;
// 		
// 		return id;
// 
// 	}
// 	else
// 		return itr->second;
// }
// 
// 
// JParticleEffect *JResourceManager::GetParticleEffect(const string &effectName)
// {
// 	map<string, int>::iterator itr = mParticleEffectMap.find(effectName);
// 
// 	if (itr == mParticleEffectMap.end())
// 		return NULL;
// 	else
// 		return mParticleEffectList[itr->second];
// }
// 
// 
// JParticleEffect *JResourceManager::GetParticleEffect(int id)
// {
// 	if (id >=0 && id < (int)mParticleEffectList.size())
// 		return mParticleEffectList[id];
// 	else
// 		return NULL;
// }
// 
// 
// 
// int JResourceManager::RegisterMotionEmitter(const string &emitterName)
// {
// 	map<string, int>::iterator itr = mMotionEmitterMap.find(emitterName);
// 
// 	if (itr == mMotionEmitterMap.end())
// 	{
// 		string path = mResourceRoot + emitterName;
// 		printf("creating effect:%s\n", path.c_str());
// 		JMotionEmitter *emitter = new JMotionEmitter();
// 
// 		if (emitter == NULL)
// 			return INVALID_ID;
// 
// 		emitter->LoadMotionML(path.c_str());
// 
// 		int id = mMotionEmitterList.size();
// 		mMotionEmitterList.push_back(emitter);
// 
// 		mMotionEmitterMap[emitterName] = id;
// 		
// 		return id;
// 
// 	}
// 	else
// 		return itr->second;
// }
// 
// 
// JMotionEmitter *JResourceManager::GetMotionEmitter(const string &emitterName)
// {
// 	map<string, int>::iterator itr = mMotionEmitterMap.find(emitterName);
// 
// 	if (itr == mMotionEmitterMap.end())
// 		return NULL;
// 	else
// 		return mMotionEmitterList[itr->second];
// }
// 
// 
// JMotionEmitter *JResourceManager::GetMotionEmitter(int id)
// {
// 	if (id >=0 && id < (int)mMotionEmitterList.size())
// 		return mMotionEmitterList[id];
// 	else
// 		return NULL;
// }
