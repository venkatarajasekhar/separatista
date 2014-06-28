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

#include "mandaterelatedinformation.h"

IMPLEMENT_TAG(MandateRelatedInformation8, MndtId)
IMPLEMENT_TAG(MandateRelatedInformation8, DtOfSgntr)
IMPLEMENT_TAG(MandateRelatedInformation8, AmdmntInd)
IMPLEMENT_TAG(MandateRelatedInformation8, ElctrncSgntr)
IMPLEMENT_TAG(MandateRelatedInformation8, FrstColltnDt)
IMPLEMENT_TAG(MandateRelatedInformation8, FnlColltnDt)
IMPLEMENT_TAG(MandateRelatedInformation8, Frqcy)

IMPLEMENT_CONSTRUCTOR(MandateRelatedInformation8)
{}

BEGIN_IMPLEMENT_TAG_ENUM(MandateRelatedInformation8, Separatista::Frequency6Code, Frequency, Frqcy)
IMPLEMENT_TAG_ENUM(Separatista::Frequency6Code::Annual, "YEAR")
IMPLEMENT_TAG_ENUM(Separatista::Frequency6Code::Monthly, "MNTH")
IMPLEMENT_TAG_ENUM(Separatista::Frequency6Code::Quarterly, "QURT")
IMPLEMENT_TAG_ENUM(Separatista::Frequency6Code::SemiAnnual, "MIAN")
IMPLEMENT_TAG_ENUM(Separatista::Frequency6Code::Weekly, "WEEK")
IMPLEMENT_TAG_ENUM(Separatista::Frequency6Code::Daily, "DAIL")
IMPLEMENT_TAG_ENUM(Separatista::Frequency6Code::Adhoc, "ADHO")
IMPLEMENT_TAG_ENUM(Separatista::Frequency6Code::IntraDay, "INDA")
IMPLEMENT_TAG_ENUM(Separatista::Frequency6Code::Fortnightly, "FRTN")
END_IMPLEMENT_TAG_ENUM

BEGIN_IMPLEMENT_ORDER(MandateRelatedInformation8)
MndtId,
DtOfSgntr,
AmdmntInd,
ElctrncSgntr,
FrstColltnDt,
FnlColltnDt,
Frqcy
END_IMPLEMENT_ORDER