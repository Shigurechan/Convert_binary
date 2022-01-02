# Convert_Model_Binary

|説明|  要素名  |  型  | 個数 |
| --------| ---- | ---- | ------------ |
| オブジェクト数 |  objectNum  |  unsigned int  | 1 |
| 頂点数 |  attributeNum  |  unsigned int  | 1 |
| 頂点情報 |  position,uv,normal  |  float  |   8|
| smooth shading |  shading  |  unsigned int  |   1|
| 鏡面反射 角度|  Ns  |  float  |   1|
| 環境光反射 成分|  Ka  |  float  |   3|
| 拡散反射 成分|  Kd  |  float  |   3|
|鏡面反射 成分 |  Ks  |  float  |   3|
| 放射成分|  Ke  |  float  |   3|
| Shininess |  Ni  |  float  |   1|
| アルファ値|  d  |  float   |   1|
| 照明モデル<br>(1で鏡面反射無効, 2で有効)|  illum  |  unsigned int  | 1|
|拡散反射テクスチャ |  map_Kd  |  char  |   50|
|透過率テクスチャ |  map_d  |  char  |   50|
| 鏡面反射テクスチャ|  map_Ks  |  char  |   50|
|鏡面反射角度テクスチャ |  map_Ns  |  char  |   50|
|反射テクスチャ |  refl  |  char  |   50|

```cpp

#define TEXTURE_FILE_PATH_SIZE ((size_t)(50))   //パスの長さ

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
    //std::vector<unsigned int>  index; //インデックス
    char materialName[TEXTURE_FILE_PATH_SIZE] = { '\0' };
    int shading;    //s要素
}
Vertex;

```








