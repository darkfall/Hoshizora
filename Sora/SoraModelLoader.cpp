//
//  SoraModelLoader.cpp
//  Sora
//
//  Created by Ruiwei Bu on 11/6/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraModelLoader.h"
#include "SoraResourceFile.h"
#include "SoraPath.h"

namespace sora {
    
    SoraModelLoader::SoraModelLoader() {
        
    }
    
    SoraModelLoader::SoraModelLoader(const StringType& file) {
        load(file);
    }
    
    SoraModelLoader::~SoraModelLoader() {
        
    }
    
    bool SoraModelLoader::load(const StringType& file) {
        SoraResourceFile data(file);
        if(data.isValid()) {
            mVertexList.clear();
            
            vertices.clear();
            texCoords.clear();
            normals.clear();
            
            mFaceSize = 0;
            
            SoraPath path(file.get());
            
            if(path.getExtension() == "obj") {
                parseObj((const char*)data, data.size());
            }
            return true;
        }
        return false;
    }
    
    size_t SoraModelLoader::getFaceSize() const {
        return mFaceSize;
    }
    
    size_t SoraModelLoader::getVertexSize() const {
        return mVertexList.size();
    }
    
    const SoraArray<SoraVertex>& SoraModelLoader::getVertexList() const {
        return mVertexList;
    }
    
    void SoraModelLoader::parseObjLine(const char* str) {
        if(str[0] == 'v' && str[1] == ' ') {
            float x, y, z;
            sscanf(&str[3], "%f %f %f", &x, &y, &z);
            
            Vertex vtx;
            vtx.x = x;
            vtx.y = y;
            vtx.z = z;
            
            vertices.push_back(vtx);
            
        } else if(str[0] == 'v' && str[1] == 't') {
            float tx, ty, tw;
            sscanf(&str[4], "%f %f", &tx, &ty);
            
            TexCoord tc;
            tc.x = tx;
            tc.y = ty;
            
            texCoords.push_back(tc);
        } else if(str[0] == 'v' && str[1] == 'n') {
            float x, y, z;
            sscanf(&str[3], "%f %f %f", &x, &y, &z);
            
            Vertex n;
            n.x = x;
            n.y = y;
            n.z = z;
            
            normals.push_back(n);
            
        } else if(str[0] == 'f') {
            mFaceSize++;
            
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
                        vertex.x = vertices[id].x;
                        vertex.y = vertices[id].y;
                        vertex.z = vertices[id].z;
                        
                        tmp.push_back(vertex);
                        break;
                    }
                    case 1: {
                        int id, id1; char slash;
                        sscanf(&str[start], "%i%c%i", &id, &slash, &id1);
                        id -= 1;
                        id1 -= 1;
                        
                        SoraVertex vertex;
                        vertex.x = vertices[id].x;
                        vertex.y = vertices[id].y;
                        vertex.z = vertices[id].z;
                        vertex.tx = texCoords[id1].x;
                        vertex.ty = texCoords[id1].y;
                        
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
                            vertex.x = vertices[id].x;
                            vertex.y = vertices[id].y;
                            vertex.z = vertices[id].z;
                            
                            tmp.push_back(vertex);
                        } else {
                            int id, id1, id2; char s1, s2;
                            sscanf(&str[start], "%i%c%i%c%i", &id, &s1, &id1, &s2, &id2);
                            id -=1;
                            id1 -= 1;
                            id2 -= 1;
                            
                            SoraVertex vertex;
                            vertex.x = vertices[id].x;
                            vertex.y = vertices[id].y;
                            vertex.z = vertices[id].z;
                            vertex.tx = texCoords[id1].x;
                            vertex.ty = texCoords[id1].y;
                            
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
                    mVertexList.push_back(tmp[i]);
            } else {
                for(int i=0; i<tmp.size()-2; ++i) {
                    mVertexList.push_back(tmp[i]);
                    mVertexList.push_back(tmp[i+1]);
                    mVertexList.push_back(tmp[i+2]);
                }
            }
        }
    }
    
    void SoraModelLoader::parseObj(const char* data, uint32 length) {
        char buffer[512];
        
        int start = 0, end = start;
        while(end < length) {
            while(data[end] != '\n' && end < length) ++end;
            
            memset(buffer, 0, 512);
            memcpy(&buffer[0], &data[start], end-start);
            buffer[end-start] = '\0';
            
            parseObjLine(buffer);
            
            end = end+1;
            start = end;
        }
        
        
        
    }
    
} // namespace sora
