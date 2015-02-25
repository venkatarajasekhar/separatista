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

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include "separatista/separatista.h"
#include "separatista/pain/directdebittransaction.h"
#include "separatista/debug/debug.h"
#include "separatista/validator.h"
#include "separatista/documentiterator.h"

using namespace Separatista;
using namespace Separatista::pain_008_001;

DrctDbtTx::DrctDbtTx(const ElementOptions options) :
BranchElement(TEXT("DrctDbtTx"), options),
m_MndtRltdInf(Element::Optional)
{
	DEBUG_METHOD
}

DOMElement* DrctDbtTx::toDOMDocument(Separatista::DOMDocument *pDocument, DOMElement *pParent, const ErrorOptions errorOptions)
{
	DEBUG_METHOD
	DOMElement *pElement = createElement(pDocument, pParent);

	if (pElement)
	{
		m_MndtRltdInf.toDOMDocument(pDocument, pElement, errorOptions);
	}

	return pElement;
}

void DrctDbtTx::fromDOMDocument(DOMDocumentIterator &elementIterator, const ErrorOptions errorOptions)
{
	DEBUG_METHOD

	elementIterator.fromDOMDocument(m_MndtRltdInf, errorOptions);
}

