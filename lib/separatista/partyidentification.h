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

#ifndef SEPARATISTA_PARTYIDENTIFICATION_H
#define SEPARATISTA_PARTYIDENTIFICATION_H

#include "macro.h"
#include "element.h"
#include "separatista.h"
#include "postaladdress.h"

namespace SeparatistaPrivate
{
	BEGIN_DECLARE_CLASS_SUPER(PartyIdentification32, PartyIdentification32)
	DECLARE_TAG(Name, Nm)
	DECLARE_CHILD(PostalAddress6, PostalAddress, PstlAdr)
	DECLARE_TAG(Identification, Id)
	DECLARE_TAG(CountryOfResidence, CtryOfRes)
	DECLARE_TAG(ContactDetails, CtctDtls)
	END_DECLARE_CLASS

}

#endif // !defined SEPARATISTA_PARTYIDENTIFICATION_H
