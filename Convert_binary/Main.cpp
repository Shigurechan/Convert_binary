#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include <time.h>
#include <sys/timeb.h> 

#define LINE_BUFFER ((size_t)(500))

#define TEXTURE_FILE_PATH_SIZE ((size_t)(50))

typedef struct vertex_attribute
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;

}VertexAttribute;


typedef struct material_file
{
    char materialName[TEXTURE_FILE_PATH_SIZE] = { '\0' };
    float Ns = 0.0f;
    glm::vec3 Ka = glm::vec3(0, 0, 0);
    glm::vec3 Kd = glm::vec3(0, 0, 0);
    glm::vec3 Ks = glm::vec3(0, 0, 0);
    glm::vec3 Ke = glm::vec3(0, 0, 0);
    float Ni = 0.0f;
    float d = 0.0f;
    int illum = 1;
    char map_Kd[TEXTURE_FILE_PATH_SIZE] = { '\0' };
    char map_Ks[TEXTURE_FILE_PATH_SIZE] = { '\0' };
    char map_d[TEXTURE_FILE_PATH_SIZE] = { '\0' };
    char map_Ns[TEXTURE_FILE_PATH_SIZE] = { '\0' };
    char refl[TEXTURE_FILE_PATH_SIZE] = { '\0' };

}
Material_File;

typedef struct vertex
{
    std::vector<VertexAttribute> attribute;
    //std::vector<unsigned int>  index;
    char materialName[TEXTURE_FILE_PATH_SIZE] = { '\0' };
    int shading;
}
Vertex;

// ##################################### ファイルパス　取得 ##################################### 
void  GetFolderPath(const char* path, char* outPath, size_t outPathSize)
{
    for (int i = strlen(path); i > 0; i--)
    {
        if (path[i] == '/')
        {
            strncpy_s(outPath, outPathSize, path, i + 1);
            break;
        }
    }
}

// ##################################### ディレクトリ名を取得 ##################################### 
void GetFolderName(const char* directoryPath, char* outPath, size_t outPathSize)
{
    for (int i = strlen(directoryPath); i > 0; i--)
    {
        if (directoryPath[i] == '/')
        {
            strncpy_s(outPath, outPathSize, directoryPath + i, strlen(directoryPath));
            break;
        }
    }
}


// ##################################### .objファイル読み込み #####################################
std::shared_ptr<std::vector<Vertex>> GetVertex(const char* path)
{

    std::vector<Vertex> polygon;
    polygon.resize(0);

    std::vector<int> vertexIndex;
    std::vector<int> uvIndex;
    std::vector<int> normalIndex;

    std::vector<glm::vec3> vertex;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normal;

    FILE* file;
    fopen_s(&file, path, "r");

    if (file == NULL)
    {
        std::cerr << ".objファイルが開けません: " << path << std::endl;
        assert(0);
    }
    else
    {
        Vertex temp;
        bool first = false;
        bool second = false;

        while (true)
        {

            char line[500] = { '\0' };

            int res = fscanf_s(file, "%s", line, (unsigned int)LINE_BUFFER);

            if (res == EOF)
            {
                if (second == false)
                {
                    for (unsigned int i = 0; i < vertexIndex.size(); i++)
                    {
                        unsigned int vi = vertexIndex[i];
                        unsigned int ui = uvIndex[i];
                        unsigned int ni = normalIndex[i];

                        glm::vec3 v = vertex[vi - 1];
                        glm::vec2 u = uv[ui - 1];
                        glm::vec3 n = normal[ni - 1];

                        VertexAttribute attrib;

                        attrib.position[0] = v.x;
                        attrib.position[1] = v.y;
                        attrib.position[2] = v.z;

                        attrib.uv[0] = u.x;
                        attrib.uv[1] = u.y;


                        attrib.normal[0] = n.x;
                        attrib.normal[1] = n.y;
                        attrib.normal[2] = n.z;

                        temp.attribute.push_back(attrib);

                    }

                    polygon.push_back(temp);
                    temp.attribute.resize(0);
                }

                break;

            }

            if (strcmp(line, "o") == 0)
            {
                if (first == true)
                {
                    char str[LINE_BUFFER] = { '\0' };
                    fscanf_s(file, "%s", str, (unsigned int)LINE_BUFFER);
                    //                printf("%s\n",str);

                    for (unsigned int i = 0; i < vertexIndex.size(); i++)
                    {
                        unsigned int vi = vertexIndex[i];
                        unsigned int ui = uvIndex[i];
                        unsigned int ni = normalIndex[i];

                        glm::vec3 v = vertex[vi - 1];
                        glm::vec2 u = uv[ui - 1];
                        glm::vec3 n = normal[ni - 1];

                        VertexAttribute attrib{ glm::vec3(0,0,0),glm::vec2(0,0),glm::vec3(0,0,0) };

                        attrib.position[0] = v.x;
                        attrib.position[1] = v.y;
                        attrib.position[2] = v.z;

                        attrib.uv[0] = u.x;
                        attrib.uv[1] = u.y;

                        attrib.normal[0] = n.x;
                        attrib.normal[1] = n.y;
                        attrib.normal[2] = n.z;

                        temp.attribute.push_back(attrib);
                    }
                    polygon.push_back(temp);
                    temp.attribute.resize(0);
                    second = true;
                }

                if (first == false)
                {

                    first = true;
                }
            }
            else if (strcmp(line, "v") == 0)
            {
                glm::vec3 v;
                fscanf_s(file, "%f %f %f", &v.x, &v.y, &v.z);

                vertex.push_back(v);
                //                printf("v %f %f %f \n", v.x, v.y, v.z);
            }
            else if (strcmp(line, "vt") == 0)
            {
                glm::vec2 u;
                fscanf_s(file, "%f %f", &u.x, &u.y);

                u.x = u.x;
                u.y = 1.0f - u.y;
                //u.y = u.y;



                uv.push_back(u);
                //               printf("vt %f %f \n", u.x, u.y);
            }
            else if (strcmp(line, "vn") == 0)
            {
                glm::vec3 n;
                fscanf_s(file, "%f %f %fn", &n.x, &n.y, &n.z);

                normal.push_back(n);
                //                printf("vn %f %f %f \n",n.x,n.y,n.z);
            }
            else if (strcmp(line, "f") == 0)
            {
                unsigned int v[3], u[3], n[3];
                int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%dn", &v[0], &u[0], &n[0], &v[1], &u[1], &n[1], &v[2], &u[2], &n[2]);

                vertexIndex.push_back(v[0]);
                vertexIndex.push_back(v[1]);
                vertexIndex.push_back(v[2]);

                uvIndex.push_back(u[0]);
                uvIndex.push_back(u[1]);
                uvIndex.push_back(u[2]);

                normalIndex.push_back(n[0]);
                normalIndex.push_back(n[1]);
                normalIndex.push_back(n[2]);

            }
            else if (strcmp(line, "usemtl") == 0)
            {
                char str[LINE_BUFFER] = { '\0' };
                char string[LINE_BUFFER] = { '\0' };
                fscanf_s(file, "%s", str, (unsigned int)LINE_BUFFER);
                
                memcpy(temp.materialName, str, TEXTURE_FILE_PATH_SIZE);
//                    printf("newmtl %s\n", temp.materialName);
            }
            else if (strcmp(line, "s") == 0)
            {
                //smooth shadingをするかどうか？    

                char str[LINE_BUFFER];
                char number[LINE_BUFFER];
                fscanf_s(file, "%s", str, (unsigned int)LINE_BUFFER);
                strncpy_s(number, str, LINE_BUFFER);
                int num = atoi(number);

                temp.shading = num;
                printf("s %d\n", num);
            }
        }

    }


    //    printf("polygon: %d\n",polygon.size());
    return std::make_shared<std::vector<Vertex>>(polygon);
}


// ##################################### .mtlファイル読み込み #####################################
std::shared_ptr<std::vector<Material_File>> GetMaterial(const char* path)
{

    std::vector<Material_File> material;
    material.resize(0);

    std::vector<std::string> textureName;
    textureName.resize(0);




    char mtlName[LINE_BUFFER] = {'\0'};

    FILE* file;
    fopen_s(&file, path, "r");

    bool isFirst = true;
    if (file == NULL)
    {
        std::cerr << ".mtlファイルが開けません: " << path << std::endl;
        assert(0);
    }
    else
    {
        Material_File mtl;

        while (true)
        {
            char line[LINE_BUFFER] = { '\0' };
            int res = fscanf_s(file, "%s", line, (unsigned int)LINE_BUFFER);
            // printf("%s\n",line);
            if (res == EOF)
            {
                memcpy(mtl.materialName, mtlName, TEXTURE_FILE_PATH_SIZE);
                material.push_back(mtl);

                break;
            }

            if (strncmp(line, "newmtl", strlen("newmtl")) == 0) //マテリアル
            {
                if (isFirst == false)
                {
                    memcpy(mtl.materialName, mtlName, TEXTURE_FILE_PATH_SIZE);
                    material.push_back(mtl);

//                    printf("newmtl %s\n", mtl.materialName);
                }

                if (isFirst == true)
                {
                    isFirst = false;
                }

                fscanf_s(file, "%s", mtlName, LINE_BUFFER);

            }
            else if (strncmp(line, "Ns", strlen("Ns")) == 0)    //鏡面反射角度
            {
                float ns = 0;
                fscanf_s(file, "%f", &ns);

                mtl.Ns = ns;
                //                printf("Ns %f\n", mtl.Ns);
            }
            else if (strncmp(line, "Ka", strlen("Ka")) == 0)    //環境光反射成分
            {
                glm::vec3 vec = glm::vec3(0, 0, 0);
                fscanf_s(file, "%f", &vec.x);
                fscanf_s(file, "%f", &vec.y);
                fscanf_s(file, "%f", &vec.z);

                mtl.Ka = vec;
                //                printf("Ka %f %f %f \n", mtl.Ka.x, mtl.Ka.y, mtl.Ka.z);
            }
            else if (strncmp(line, "Kd", strlen("Kd")) == 0)    //拡散反射成分
            {
                glm::vec3 vec = glm::vec3(0, 0, 0);
                fscanf_s(file, "%f", &vec.x);
                fscanf_s(file, "%f", &vec.y);
                fscanf_s(file, "%f", &vec.z);

                mtl.Kd = vec;
                //                printf("Kd %f %f %f \n", mtl.Kd.x, mtl.Kd.y, mtl.Kd.z);
            }
            else if (strncmp(line, "Ks", strlen("Ks")) == 0)    //鏡面反射成分
            {
                glm::vec3 vec = glm::vec3(0, 0, 0);
                fscanf_s(file, "%f", &vec.x);
                fscanf_s(file, "%f", &vec.y);
                fscanf_s(file, "%f", &vec.z);

                mtl.Ks = vec;
                //                printf("Kd %f %f %f \n", mtl.Ks.x, mtl.Ks.y, mtl.Ks.z);
            }
            else if (strncmp(line, "Ke", strlen("Ke")) == 0)    //放射
            {
                glm::vec3 vec = glm::vec3(0, 0, 0);
                fscanf_s(file, "%f", &vec.x);
                fscanf_s(file, "%f", &vec.y);
                fscanf_s(file, "%f", &vec.z);

                mtl.Ke = vec;
                //                printf("Ke %f %f %f \n", mtl.Ke.x, mtl.Ke.y, mtl.Ke.z);
            }
            else if (strncmp(line, "Ni", strlen("Ni")) == 0)    //Shininess
            {
                float ni = 0;
                fscanf_s(file, "%f", &ni);

                mtl.Ni = ni;
                //                printf("Ni %f\n", mtl.Ni);
            }
            else if (strncmp(line, "d", strlen("d")) == 0)  //アルファ値
            {
                float d = 0;
                fscanf_s(file, "%f", &d);

                mtl.d = d;
                //                printf("d %f\n", mtl.d);
            }
            else if (strncmp(line, "illum", strlen("illum")) == 0)  //照明モデル(1で鏡面反射無効, 2で有効)
            {
                int illum = 0;
                fscanf_s(file, "%d", &illum);

                mtl.illum = illum;
                //                printf("d %d\n", mtl.illum);
            }
            else if (strncmp(line, "map_Kd", strlen("map_Kd")) == 0)    //拡散反射マップ
            {
                char texture[LINE_BUFFER] = { '\0' };
                fscanf_s(file, "%s", &texture, (unsigned int)LINE_BUFFER);
                char filePath[LINE_BUFFER] = { '\0' };
                char newPath[LINE_BUFFER] = { '\0' };


                GetFolderPath(path, filePath, LINE_BUFFER);
                strcat_s(filePath, texture);

                memcpy(mtl.map_Kd, filePath, TEXTURE_FILE_PATH_SIZE);

                                printf("mtl.map_Kd  %s\n", mtl.map_Kd);  
            }
            else if (strncmp(line, "map_Ks", strlen("map_Ks")) == 0)    //鏡面反射マップ
            {
                char texture[LINE_BUFFER] = { '\0' };
                fscanf_s(file, "%s", &texture, (unsigned int)LINE_BUFFER);
                char filePath[LINE_BUFFER] = { '\0' };
                char newPath[LINE_BUFFER] = { '\0' };


                GetFolderPath(path, filePath, LINE_BUFFER);
                strcat_s(filePath, texture);

                memcpy(mtl.map_Ks, filePath, TEXTURE_FILE_PATH_SIZE);

                                printf("mtl.map_Ks  %s\n", mtl.map_Ks);  
            }
            else if (strncmp(line, "map_d", strlen("map_d")) == 0)  //透過率マップ
            {
                char texture[LINE_BUFFER] = { '\0' };
                fscanf_s(file, "%s", &texture, (unsigned int)LINE_BUFFER);
                char filePath[LINE_BUFFER] = { '\0' };
                char newPath[LINE_BUFFER] = { '\0' };

                GetFolderPath(path, filePath, LINE_BUFFER);
                strcat_s(filePath, texture);

                memcpy(mtl.map_d, filePath, TEXTURE_FILE_PATH_SIZE);

                                printf("mtl.map_d  %s\n", mtl.map_d);
            }
            else if (strncmp(line, "map_Ns", strlen("map_Ns")) == 0)  //鏡面反射角度マップ
            {
                char texture[LINE_BUFFER] = { '\0' };
                fscanf_s(file, "%s", &texture, (unsigned int)LINE_BUFFER);
                char filePath[LINE_BUFFER] = { '\0' };
                char newPath[LINE_BUFFER] = { '\0' };

                GetFolderPath(path, filePath, LINE_BUFFER);
                strcat_s(filePath, texture);

                memcpy(mtl.map_Ns, filePath, TEXTURE_FILE_PATH_SIZE);

                                printf("mtl.map_Ns  %s\n", mtl.map_Ns);
            }
            else if (strncmp(line, "refl", strlen("refl")) == 0)  //球形反射マップ
            {
                char texture[LINE_BUFFER] = { '\0' };
                fscanf_s(file, "%s", &texture, (unsigned int)LINE_BUFFER);
                char filePath[LINE_BUFFER] = { '\0' };
                char newPath[LINE_BUFFER] = { '\0' };

                GetFolderPath(path, filePath, LINE_BUFFER);
                strcat_s(filePath, texture);

                memcpy(mtl.refl, filePath, TEXTURE_FILE_PATH_SIZE);

                                printf("mtl.refl  %s\n", mtl.refl);
            }



        }
    }

    return std::make_shared<std::vector<Material_File>>(material);
}

int main()
{
    timeb start, end;


    std::shared_ptr<std::vector<Vertex>> vertex = GetVertex("tifa.obj");
    std::shared_ptr<std::vector<Material_File>> material = GetMaterial("tifa.mtl");







    FILE *file;
    if (fopen_s(&file, "tifa.model", "wb") == 0)
    {  
        ftime(&start);
        unsigned int objectNum = vertex->size();
        fwrite(&objectNum, sizeof(unsigned int), 1, file); //オブジェクト数

        for (int i = 0; i < vertex->size(); i++)
        {
            int vertedNum = vertex->at(i).attribute.size() + 1;
            fwrite(&vertedNum, sizeof(unsigned int), 1, file); //頂点数
            for (std::vector<VertexAttribute>::const_iterator itr = vertex->at(i).attribute.begin(); itr != vertex->at(i).attribute.end(); itr++)
            {                      
                fwrite(&itr->position, sizeof(float), 3, file);
                fwrite(&itr->uv, sizeof(float), 2, file);
                fwrite(&itr->normal , sizeof(float), 3, file); 
            }

            fwrite(&vertex->at(i).shading, sizeof(unsigned int), 1, file);  //s 要素

            for (int j = 0; j < material->size(); j++)
            {
                if (strcmp(vertex->at(i).materialName,material->at(j).materialName) == 0)
                {
                
                    fwrite(&material->at(j).Ns, sizeof(float), 1, file);

                    fwrite(&material->at(j).Ka, sizeof(float), 3, file);
                    fwrite(&material->at(j).Kd, sizeof(float), 3, file);
                    fwrite(&material->at(j).Ks, sizeof(float), 3, file);
                    fwrite(&material->at(j).Ke, sizeof(float), 3, file);

                    fwrite(&material->at(j).Ni, sizeof(float), 1, file);
                    fwrite(&material->at(j).d, sizeof(float), 1, file);
                    fwrite(&material->at(j).illum, sizeof(int), 1, file);

                    fwrite(&material->at(j).map_Kd, sizeof(char), TEXTURE_FILE_PATH_SIZE, file);
                    fwrite(&material->at(j).map_Ks, sizeof(char), TEXTURE_FILE_PATH_SIZE, file);
                    fwrite(&material->at(j).map_d, sizeof(char), TEXTURE_FILE_PATH_SIZE, file);
                    fwrite(&material->at(j).map_Ns, sizeof(char), TEXTURE_FILE_PATH_SIZE, file);
                    fwrite(&material->at(j).refl, sizeof(char), TEXTURE_FILE_PATH_SIZE, file);

                }
            }
        }


        ftime(&end);
        float diff = (end.time - start.time) + ((end.millitm - start.millitm) * 0.0001);
        printf("\n\n        Convert Time %f \n\n", diff);

    }

    fclose(file);
    
	return 0;
}