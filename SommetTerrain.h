#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;
namespace PM3D {

   struct CSommetTerrain {
      CSommetTerrain() = default;
      CSommetTerrain(const XMFLOAT3& position, const XMFLOAT3& normal);

      static UINT numElements;
      static D3D11_INPUT_ELEMENT_DESC layout[];

      XMFLOAT3 position;
      XMFLOAT3 normale;
   };

}
