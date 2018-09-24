#include "SommetTerrain.h"

namespace PM3D{

   D3D11_INPUT_ELEMENT_DESC CSommetTerrain::layout[] =
   {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
   };

   UINT CSommetTerrain::numElements = ARRAYSIZE(CSommetTerrain::layout);

   CSommetTerrain::CSommetTerrain(const XMFLOAT3 & pos, const XMFLOAT3 & normal)
   {
      position = pos;
      normale = normal;
   }
}