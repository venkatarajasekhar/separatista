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

#include "separatista/separatista.h"
#include "separatista/xerces_types.h"
#include "separatista/element.h"
#include "separatista/partyidentification.h"

#ifndef SEPARATISTA_PAIN_GROUPHEADER_H
#define SEPARATISTA_PAIN_GROUPHEADER_H

namespace Separatista
{
	namespace pain_008_001
	{
		class SEPARATISTA_EXTERN GrpHdr : public BranchElement
		{
		public:
			GrpHdr(const ElementOptions options);

			DOMElement* toDOMDocument(DOMDocument *pDocument, DOMElement *pParent, const ErrorOptions errorOptions = ThrowExceptions);

			void fromDOMDocument(DOMDocumentIterator &DocumentIterator, const ErrorOptions errorOptions = ThrowExceptions);

			LeafElement m_MsgId;
			LeafElement m_CreDtTm;
			LeafElement m_NbOfTxs;
			LeafElement m_CtrlSum;
			PartyIdentification43 m_InitgPty;

		};
	}
}

#endif // !defined SEPARATISTA_PAIN_GROUPHEADER_H
