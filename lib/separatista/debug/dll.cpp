/***************************************************************************
*   Copyright (C) 2014 by Okkel Klaver   *
*   info@vanhetland.nl   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include <windows.h>
#include "debug.h"

/**
DllMain
*/

BOOL WINAPI DllMain(
	HINSTANCE hinstDll,
	DWORD fdwReason,
	LPVOID lpvReserved)
{
	DEBUG_METHOD

	// Get reason
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			return Separatista::Debug::MemDebug::init() ? TRUE : FALSE;
			break;
		case DLL_PROCESS_DETACH:
			return Separatista::Debug::MemDebug::exit() ? TRUE : FALSE ;
			break;
	}

	return TRUE;
}
