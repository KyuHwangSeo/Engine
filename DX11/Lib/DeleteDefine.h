#pragma once
//---------------------------------------------------------------------------------------
// Convenience macro for releasing COM objects.
//---------------------------------------------------------------------------------------

#define RELEASE_COM(x) { if(x != nullptr){ x->Release(); x = nullptr; } }
#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

//---------------------------------------------------------------------------------------
// Convenience macro for deleting objects.
//---------------------------------------------------------------------------------------

#define SAFE_DELETE(x) { if(x != nullptr) {delete x; x = nullptr;} }
#define RESET_COM(x) { if(x != nullptr) {x.Reset();} }

