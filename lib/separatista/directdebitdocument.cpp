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

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>

#include "separatista.h"
#include "directdebitdocument.h"
#include "iban/iban.h"

#include <string>

using namespace xercesc;
using namespace SeparatistaPrivate;
using Separatista::DirectDebitDocument;

#define IMPLEMENT_CONSTRUCTOR(name) \
	name::name(DOMDocument *pDocument, Element *pParent, DOMElement *pElement, const wchar_t *pTagName) \
	:Element(pDocument, pParent, pElement, pTagName)

#define IMPLEMENT_CHILD(child, tag) \
	,m_##child(pDocument, this, NULL, tag)

#define IMPLEMENT_TAG(name, tag) \
	const wchar_t* name::tag = L#tag;

#define BEGIN_IMPLEMENT_ORDER(name) \
	const wchar_t* const* name::getOrder() \
	{ \
	static const wchar_t* order[] = { \

#define END_IMPLEMENT_ORDER  \
	, NULL }; \
	return order; }

IMPLEMENT_TAG(LocalInstrument2Choise, Cd)
IMPLEMENT_TAG(LocalInstrument2Choise, Prtry)

IMPLEMENT_CONSTRUCTOR(LocalInstrument2Choise)
{
}

BEGIN_IMPLEMENT_ORDER(LocalInstrument2Choise)
	Cd,
	Prtry
END_IMPLEMENT_ORDER

IMPLEMENT_TAG(ServiceLevel8Choise, Cd)
IMPLEMENT_TAG(ServiceLevel8Choise, Prtry)

IMPLEMENT_CONSTRUCTOR(ServiceLevel8Choise)
{
}

BEGIN_IMPLEMENT_ORDER(ServiceLevel8Choise)
		Cd,
		Prtry
END_IMPLEMENT_ORDER

IMPLEMENT_TAG(PaymentTypeInformation20, InstrPty)
IMPLEMENT_TAG(PaymentTypeInformation20, SvcLvl)
IMPLEMENT_TAG(PaymentTypeInformation20, LclInstrm)
IMPLEMENT_TAG(PaymentTypeInformation20, SeqTp)
IMPLEMENT_TAG(PaymentTypeInformation20, CtgyPurp)

IMPLEMENT_CONSTRUCTOR(PaymentTypeInformation20)
IMPLEMENT_CHILD(ServiceLevel, PaymentTypeInformation::SvcLvl)
{
}

BEGIN_IMPLEMENT_ORDER(PaymentTypeInformation20)
		InstrPty,
		SvcLvl,
		LclInstrm,
		SeqTp,
		CtgyPurp
END_IMPLEMENT_ORDER

IMPLEMENT_TAG(CustomerDirectDebitInitiationV02, CstmrDrctDbtInitn)
IMPLEMENT_TAG(CustomerDirectDebitInitiationV02, GrpHdr)
IMPLEMENT_TAG(CustomerDirectDebitInitiationV02, PmtInf)

IMPLEMENT_CONSTRUCTOR(CustomerDirectDebitInitiationV02)
IMPLEMENT_CHILD(GroupHeader, CustomerDirectDebitInitiation::GrpHdr)
{
	// Find all PaymentInformation elements if they exist
	DOMNodeList *pNodeList;

	pNodeList = getElementsByTagName(PmtInf);
	if (pNodeList)
	{
		for (XMLSize_t i = 0; i < pNodeList->getLength(); i++)
			addPaymentInstructionInformation(new PaymentInstructionInformation(pDocument, this, (DOMElement*)pNodeList->item(i)));
	}

	moveFirst();
}

BEGIN_IMPLEMENT_ORDER(CustomerDirectDebitInitiationV02)
	GrpHdr,
	PmtInf
END_IMPLEMENT_ORDER

CustomerDirectDebitInitiationV02::~CustomerDirectDebitInitiationV02()
{
	// Delete all PaymentInstructionInformations
	while (!m_pmtInfs.empty())
	{
		delete m_pmtInfs.back();
		m_pmtInfs.pop_back();
	}
}

void CustomerDirectDebitInitiationV02::addPaymentInstructionInformation(PaymentInstructionInformation *pPmtInf)
{
	m_pmtInfs.push_back(pPmtInf);
	moveFirst();
}

bool CustomerDirectDebitInitiationV02::FEOF()
{
	if (m_pmtInfIterator != m_pmtInfs.end())
		return false;
	return true;
}

void CustomerDirectDebitInitiation::moveFirst()
{
	m_pmtInfIterator = m_pmtInfs.begin();
}

void CustomerDirectDebitInitiation::moveNext()
{
	if (m_pmtInfIterator != m_pmtInfs.end())
		++m_pmtInfIterator;
}

IMPLEMENT_TAG(GroupHeader39, MsgId)
IMPLEMENT_TAG(GroupHeader39, CreDtTm)
IMPLEMENT_TAG(GroupHeader39, Authstn)
IMPLEMENT_TAG(GroupHeader39, NbOfTxs)
IMPLEMENT_TAG(GroupHeader39, CtrlSum)
IMPLEMENT_TAG(GroupHeader39, InitgPty)
IMPLEMENT_TAG(GroupHeader39, FwdgAgt)

IMPLEMENT_CONSTRUCTOR(GroupHeader39)
IMPLEMENT_CHILD(InitiatingParty, GroupHeader::InitgPty)
{
}

BEGIN_IMPLEMENT_ORDER(GroupHeader39)
	MsgId,
	CreDtTm,
	Authstn,
	NbOfTxs,
	CtrlSum,
	InitgPty,
	FwdgAgt
END_IMPLEMENT_ORDER

IMPLEMENT_TAG(PaymentInstructionInformation4, PmtInfId)
IMPLEMENT_TAG(PaymentInstructionInformation4, PmtMtd)
IMPLEMENT_TAG(PaymentInstructionInformation4, BtchBookg)
IMPLEMENT_TAG(PaymentInstructionInformation4, NbOfTxs)
IMPLEMENT_TAG(PaymentInstructionInformation4, CtrlSum)
IMPLEMENT_TAG(PaymentInstructionInformation4, PmtTpInf)
IMPLEMENT_TAG(PaymentInstructionInformation4, ReqdColltnDt)
IMPLEMENT_TAG(PaymentInstructionInformation4, Cdtr)
IMPLEMENT_TAG(PaymentInstructionInformation4, CdtrAcct)
IMPLEMENT_TAG(PaymentInstructionInformation4, CdtrAgt)
IMPLEMENT_TAG(PaymentInstructionInformation4, CdtrAgtAcct)
IMPLEMENT_TAG(PaymentInstructionInformation4, UlmtCdtr)
IMPLEMENT_TAG(PaymentInstructionInformation4, ChrgBr)
IMPLEMENT_TAG(PaymentInstructionInformation4, ChrgsAcct)
IMPLEMENT_TAG(PaymentInstructionInformation4, ChrgsAgtAcct)
IMPLEMENT_TAG(PaymentInstructionInformation4, CdtrSchmeId)
IMPLEMENT_TAG(PaymentInstructionInformation4, DrctDbtTxInf)

const wchar_t* PaymentInstructionInformation4::DirectDebit = L"DD";

IMPLEMENT_CONSTRUCTOR(PaymentInstructionInformation4)
IMPLEMENT_CHILD(PaymentTypeInformation,PaymentInstructionInformation::PmtTpInf)
{
}

BEGIN_IMPLEMENT_ORDER(PaymentInstructionInformation4)
		PmtInfId,
		PmtMtd,
		BtchBookg,
		NbOfTxs,
		CtrlSum,
		PmtTpInf,
		ReqdColltnDt,
		Cdtr,
		CdtrAcct,
		CdtrAgt,
		CdtrAgtAcct,
		UlmtCdtr,
		ChrgBr,
		ChrgsAcct,
		ChrgsAgtAcct,
		CdtrSchmeId,
		DrctDbtTxInf
END_IMPLEMENT_ORDER

IMPLEMENT_TAG(PartyIdentification32, Nm)
IMPLEMENT_TAG(PartyIdentification32, PstlAdr)
IMPLEMENT_TAG(PartyIdentification32, Id)
IMPLEMENT_TAG(PartyIdentification32, CtryOfRes)
IMPLEMENT_TAG(PartyIdentification32, CtctDtls)

IMPLEMENT_CONSTRUCTOR(PartyIdentification32)
{
}

BEGIN_IMPLEMENT_ORDER(PartyIdentification32)
		Nm,
		PstlAdr,
		Id,
		CtryOfRes,
		CtctDtls
END_IMPLEMENT_ORDER

const wchar_t* DirectDebitDocument::NamespaceURI = L"urn:iso:std:iso:20022:tech:xsd:pain.008.001.02";

DirectDebitDocument::DirectDebitDocument()
{
	DOMDocument *pDocument;
	DOMElement *pElement, *pDocumentElement;

	try
	{
		DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(L"core");
		if (!impl)
			return;

		pDocument = impl->createDocument(NamespaceURI, Document, NULL);
		if (!pDocument)
			return;
		setDOMDocument(pDocument);

		pDocumentElement = pDocument->getDocumentElement();
		if (!pDocumentElement)
			return;

		pElement = pDocument->createElement(CustomerDirectDebitInitiation::CstmrDrctDbtInitn);
		if (!pElement)
			return;
		pDocumentElement->appendChild(pElement);
		
		m_pCstmrDrctDbtInitn = new CustomerDirectDebitInitiation(pDocument, NULL, pElement);
	}
	catch (const DOMException &e)
	{
		setErrorMessage(e.getMessage());
	}
	catch (...)
	{
	}
}

DirectDebitDocument::DirectDebitDocument(xercesc::DOMDocument *pDocument)
{
	DOMElement *pElement;

	setDOMDocument(pDocument);

	m_pCstmrDrctDbtInitn = NULL;

	pElement = pDocument->getDocumentElement();
	if (!pElement)
		return;
	pElement = pElement->getFirstElementChild();
	if (XMLString::compareString(pElement->getTagName(), CustomerDirectDebitInitiation::CstmrDrctDbtInitn) != 0)
		return;

	m_pCstmrDrctDbtInitn = new CustomerDirectDebitInitiation(pDocument, NULL, pElement);
}

DirectDebitDocument::~DirectDebitDocument()
{
	if (m_pCstmrDrctDbtInitn)
		delete m_pCstmrDrctDbtInitn;
}

const Separatista::SeparatistaDocument::DocumentType DirectDebitDocument::getDocumentType() const
{
	return SeparatistaDocument::DirectDebitDocumentType;
}

const wchar_t* DirectDebitDocument::getMessageIdentification()
{
	if (!m_pCstmrDrctDbtInitn)
		return NULL;

	return m_pCstmrDrctDbtInitn->getGroupHeader().getMessageIdentification();
}

void DirectDebitDocument::setMessageIdentification(const wchar_t *pValue)
{
	if (m_pCstmrDrctDbtInitn)
		m_pCstmrDrctDbtInitn->getGroupHeader().setMessageIdentification(pValue);
}

time_t DirectDebitDocument::getCreationDateTime()
{
	const wchar_t *pDate;

	if (!m_pCstmrDrctDbtInitn)
		return -1;

	if ((pDate = m_pCstmrDrctDbtInitn->getGroupHeader().getCreationDateTime()) == NULL)
		return -1;

	return toTime(pDate);
}

void DirectDebitDocument::setCreationDateTime(const wchar_t *pValue)
{
	if (m_pCstmrDrctDbtInitn)
		m_pCstmrDrctDbtInitn->getGroupHeader().setCreationDateTime(pValue);
}

long DirectDebitDocument::getNumberOfTransactions()
{
	const wchar_t *pN;

	if (!m_pCstmrDrctDbtInitn)
		return 0;

	if ((pN = m_pCstmrDrctDbtInitn->getGroupHeader().getNumberOfTransactions()) == NULL)
		return 0;

	return toLong(pN);
}

uint64_t DirectDebitDocument::getControlSum()
{
	const wchar_t *pN;

	if (!m_pCstmrDrctDbtInitn)
		return 0;

	if ((pN = m_pCstmrDrctDbtInitn->getGroupHeader().getControlSum()) == NULL)
		return 0;

	return toUInt64(pN);
}

const wchar_t* DirectDebitDocument::getAuthorisation(unsigned long index)
{
	if (!m_pCstmrDrctDbtInitn)
		return NULL;

	return m_pCstmrDrctDbtInitn->getGroupHeader().getChildElementValue(index, GroupHeader::Authstn);
}

void DirectDebitDocument::setAuthorisation(unsigned long index, const wchar_t *pValue)
{
	if (m_pCstmrDrctDbtInitn)
		m_pCstmrDrctDbtInitn->getGroupHeader().setChildElementValue(index, GroupHeader::Authstn, pValue);
}

const wchar_t* DirectDebitDocument::getForwardingAgent()
{
	if (!m_pCstmrDrctDbtInitn)
		return NULL;

	return m_pCstmrDrctDbtInitn->getGroupHeader().getForwardingAgent();
}

void DirectDebitDocument::setForwardingAgent(const wchar_t *pValue)
{
	if (m_pCstmrDrctDbtInitn)
		m_pCstmrDrctDbtInitn->getGroupHeader().setForwardingAgent(pValue);
}

Separatista::PartyIdentification* DirectDebitDocument::getInitiatingParty()
{
	if (!m_pCstmrDrctDbtInitn)
		return NULL;

	return &(m_pCstmrDrctDbtInitn->getGroupHeader().getInitiatingParty());
}

bool DirectDebitDocument::FEOF()
{
	if (!m_pCstmrDrctDbtInitn)
		return true;

	return m_pCstmrDrctDbtInitn->FEOF();
}

void DirectDebitDocument::moveNext()
{
	if (m_pCstmrDrctDbtInitn)
		m_pCstmrDrctDbtInitn->moveNext();
}

void DirectDebitDocument::moveFirst()
{
	if (m_pCstmrDrctDbtInitn)
		m_pCstmrDrctDbtInitn->moveFirst();
}