//
//  SoraObjModelLoader.cpp
//  Sora
//
//  Created by Ruiwei Bu on 11/9/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraObjModelLoader.h"

namespace sora {
    
    SoraModel::Ptr SoraObjModelLoader::loadModelFromData(const char* data, uint32 size) {
        SoraModel* model = new SoraModel;
        
        char buffer[512];
        
        int start = 0, end = start;
        while(end < size) {
            while(data[end] != '\n' && end < size) ++end;
            
            memset(buffer, 0, 512);
            memcpy(&buffer[0], &data[start], end-start);
            buffer[end-start] = '\0';
            
            if(buffer[0] != '#')
                parseLine(buffer, model->getMesh());
            
            end = end+1;
            start = end;
        }
        
        return model;
    }
    
    void SoraObjModelLoader::parseLine(const char* str, SoraMesh* mesh) {
        if(str[0] == 'v' && str[1] == ' ') {
            float x, y, z;
            sscanf(&str[3], "%f %f %f", &x, &y, &z);
            
            float3 vtx;
            vtx[0] = x;
            vtx[1] = y;
            vtx[2] = z;
            
            mesh->mVertices.push_back(vtx);
            
        } else if(str[0] == 'v' && str[1] == 't') {
            float tx, ty;
            sscanf(&str[4], "%f %f", &tx, &ty);
            
            float2 tc;
            tc[0] = tx;
            tc[1] = ty;
            
            mesh->mUVs.push_back(tc);
            
        } else if(str[0] == 'v' && str[1] == 'n') {
            float x, y, z;
            sscanf(&str[3], "%f %f %f", &x, &y, &z);
            
            float3 n;
            n[0] = x;
            n[1] = y;
            n[2] = z;
            
            mesh->mNormals.push_back(n);
            
        } else if(str[0] == 'f') {            
            int cur = 2;
            int start = cur;
            
            SoraArray<SoraVertex> tmp;
            
            while(str[cur] != 0) {
                int slashNum = 0;
                bool notex = false;
                while(str[cur] != ' ' && str[cur] != 0) {
                    cur++;
                    if(str[cur] == '/') {
                        slashNum++;
                        if(str[cur+1] == '/')
                            notex = true;
                    }
                }
                
                switch(slashNum) {
                    case 0: {
                        int id;
                        sscanf(&str[start], "%i", &id);
                        id = id-1;
                        
                        SoraVertex vertex;
                        vertex.x = mesh->mVertices[id][0];
                        vertex.y = mesh->mVertices[id][1];
                        vertex.z = mesh->mVertices[id][2];
                        
                        tmp.push_back(vertex);
                        break;
                    }
                    case 1: {
                        int id, id1; char slash;
                        sscanf(&str[start], "%i%c%i", &id, &slash, &id1);
                        id -= 1;
                        id1 -= 1;
                        
                        SoraVertex vertex;
                        vertex.x = mesh->mVertices[id][0];
                        vertex.y = mesh->mVertices[id][1];
                        vertex.z = mesh->mVertices[id][2];
                        vertex.tx = mesh->mUVs[id1][0];
                        vertex.ty = mesh->mUVs[id1][1];
                        
                        tmp.push_back(vertex);
                        break;
                    }
                        
                    case 2: {
                        if (notex) {
                            int id, id2; char s1, s2;
                            sscanf(&str[start], "%i%c%c%i", &id, &s1, &s2, &id2);
                            id -= 1;
                            id2 -= 1;
                            
                            
                            SoraVertex vertex;
                            vertex.x = mesh->mVertices[id][0];
                            vertex.y = mesh->mVertices[id][1];
                            vertex.z = mesh->mVertices[id][2];
                            
                            tmp.push_back(vertex);
                        } else {
                            int id, id1, id2; char s1, s2;
                            sscanf(&str[start], "%i%c%i%c%i", &id, &s1, &id1, &s2, &id2);
                            id -=1;
                            id1 -= 1;
                            id2 -= 1;
                            
                            SoraVertex vertex;
                            vertex.x = mesh->mVertices[id][0];
                            vertex.y = mesh->mVertices[id][1];
                            vertex.z = mesh->mVertices[id][2];
                            vertex.tx = mesh->mUVs[id1][0];
                            vertex.ty = mesh->mUVs[id1][1];
                            
                            tmp.push_back(vertex);
                        }
                    }
                }
                
                if(str[cur] != 0) {
                    cur = cur+1;
                    start = cur;
                }
            }
            
            if(tmp.size() == 3) {
                for(int i=0; i<3; ++i)
                    mesh->mFaces.push_back(tmp[i]);
            }
        }
    }
    
    SORA_STATIC_RUN_CODE_I(sora_model_type, SoraModelLoader::GetModelLoaderFactory().registerClass<SoraObjModelLoader>("obj"));
    
} // namespace sora