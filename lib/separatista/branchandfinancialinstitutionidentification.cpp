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

#include "branchandfinancialinstitutionidentification.h"
#include "documentiterator.h"
#include "debug/debug.h"

using namespace Separatista;

FinancialInstitutionIdentification8::FinancialInstitutionIdentification8(const ElementOptions options) :
BranchElement(TEXT("FinInstnId"), options),
m_BIC(TEXT("BIC"), Validators.BICValidator, Element::Optional)
{
	DEBUG_METHOD
}

xercesc::DOMElement* FinancialInstitutionIdentification8::toDOMDocument(xercesc::DOMDocument *pDocument, xercesc::DOMElement *pParent, const ErrorOptions errorOptions)
{
	DEBUG_METHOD
	xercesc::DOMElement *pElement = createElement(pDocument, pParent);

	if (pElement)
	{
		m_BIC.toDOMDocument(pDocument, pElement, errorOptions);
	}

	return pElement;
}

void FinancialInstitutionIdentification8::fromDOMDocument(DOMDocumentIterator &elementIterator, const ErrorOptions errorOptions)
{
	DEBUG_METHOD
	elementIterator.fromDOMDocument(m_BIC, errorOptions);
}

BranchAndFinancialInstitutionIdentification5::BranchAndFinancialInstitutionIdentification5(const wchar_t *pTag, const ElementOptions options) :
BranchElement(pTag, options),
m_FinancialInstitutionIdentification(Element::Mandatory)
{
	DEBUG_METHOD
}

xercesc::DOMElement* BranchAndFinancialInstitutionIdentification5::toDOMDocument(xercesc::DOMDocument *pDocument, xercesc::DOMElement *pParent, const ErrorOptions errorOptions)
{
	DEBUG_METHOD
	xercesc::DOMElement *pElement = createElement(pDocument, pParent);

	if (pElement)
	{
		m_FinancialInstitutionIdentification.toDOMDocument(pDocument, pElement, errorOptions);
	}

	return pElement;
}

void BranchAndFinancialInstitutionIdentification5::fromDOMDocument(DOMDocumentIterator &elementIterator, const ErrorOptions errorOptions)
{
	DEBUG_METHOD

	elementIterator.fromDOMDocument(m_FinancialInstitutionIdentification, errorOptions);
}
