/***************************************************************************
*   Copyright (C) 2013 by Okkel Klaver   *
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
#include "separatista/pain/paymentinformation.h"
#include "separatista/pain/groupheader.h"
#include "separatista/partyidentification.h"
#include "separatista/elementlist.h"

#ifndef SEPARATISTA_PAIN_CUSTOMERDIRECTDEBITINITIATION_H
#define SEPARATISTA_PAIN_CUSTOMERDIRECTDEBITINITIATION_H

namespace Separatista
{
	namespace pain_008_001
	{
		class SEPARATISTA_EXTERN CstmrDrctDbtInitn : public BranchElement, protected ElementListener, public SeparatistaDocument
		{
		public:
			CstmrDrctDbtInitn();

			/**
				Construct from a dom document.
			*/
			CstmrDrctDbtInitn(DOMDocument *pDocument, const ErrorOptions errorOptions = ThrowExceptions);

			/**
			Destructor, will delete all added PmtInf's
			*/
			~CstmrDrctDbtInitn();

			DOMElement* toDOMDocument(DOMDocument *pDocument, DOMElement *pParent = NULL, const ErrorOptions errorOptions = ThrowExceptions);

			void fromDOMDocument(DOMDocumentIterator &DocumentIterator, const ErrorOptions errorOptions = ThrowExceptions);

			void elementValueChanged(Element *pElement, const wchar_t *pNewValue);

			void elementDeleted(Element *pElement);

			GrpHdr m_GrpHdr;

			void AddPmtInf(PmtInf *pPmtInf);

			void getPmtInfs(ElementList &elementList);

			PmtInf* getPmtInfById(const wchar_t *pId);

			/**
				Writes the DOM document to a local file path
				@param pPath The path to write to
				@return Error code
				*/
			IOErrorCode SaveAs(const wchar_t *pPath);

			/**
				SeparatistaDocument interface
				*/
			DocumentType getDocumentType() const { return DT_CustomerDirectDebitDocument; };

			static const wchar_t *NameSpaceURI;

		private:
			void calcSum();

			std::vector<PmtInf*> m_PmtInfs;
		};
	}
}

#endif // SEPARATISTA_PAIN_CUSTOMERDIRECTDEBITINITIATION_H
