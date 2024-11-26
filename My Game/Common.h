/// \file Common.h
/// \brief Interface for the class CCommon.

#ifndef __L4RC_GAME_COMMON_H__
#define __L4RC_GAME_COMMON_H__

#include "Defines.h"

//forward declarations to make the compiler less stroppy

class CObjectManager; 
class CRenderer;
class LParticleEngine2D;
class CTileManager;
class CPlayer;
class CUnitManager;

/// \brief The common variables class.
///
/// CCommon is a singleton class that encapsulates things that are common to
/// different game components, including game state variables. Making it a
/// singleton class means that we can avoid passing its member variables
/// around as parameters, which makes the code minisculely faster, and more
/// importantly, makes the code more readable by reducing function clutter.

class CCommon{
  protected:  
    static CRenderer* m_pRenderer; ///< Pointer to renderer.
    static CObjectManager* m_pObjectManager; ///< Pointer to object manager.
    static LParticleEngine2D* m_pParticleEngine; ///< Pointer to particle engine.
    static CTileManager* m_pTileManager; ///< Pointer to tile manager. 

	static CUnitManager* m_pUnitManager; ///< Pointer to appliance manager.

    static bool m_bDrawAABBs; ///< Draw AABB flag.
    static bool m_bGodMode; ///< God mode flag.

    static Vector2 m_vWorldSize; ///< World height and width.
//    static CPlayer* m_pPlayer; ///< Pointer to player character.
}; //CCommon

#endif //__L4RC_GAME_COMMON_H__