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

#include <comutil.h>

#include "directdebittransactioninformation.h"
#include "dispatch.cpp"
#include "util.h"

PmtId::PmtId() :
Element(TEXT("PmtId")),
m_EndToEndId(TEXT("EndToEndId"))
{

}

xercesc::DOMElement* PmtId::toDOMDocument(xercesc::DOMDocument *pDocument, xercesc::DOMElement *pParent)
{
	xercesc::DOMElement *pElement = Element::toDOMDocument(pDocument, pParent, true);

	if (pElement)
		m_EndToEndId.toDOMDocument(pDocument, pElement);

	return pElement;
}

MndtRltdInf::MndtRltdInf() :
Element(TEXT("MndtRltdInf")),
m_MndtId(TEXT("MndtId")),
m_DtOfSgntr(TEXT("DtOfSgntr"))
{
	m_DtOfSgntr.setValue(std::time(NULL));
}

xercesc::DOMElement* MndtRltdInf::toDOMDocument(xercesc::DOMDocument *pDocument, xercesc::DOMElement *pParent)
{
	xercesc::DOMElement *pElement = Element::toDOMDocument(pDocument, pParent, true);

	if (pElement)
	{
		m_MndtId.toDOMDocument(pDocument, pElement);
		m_DtOfSgntr.toDOMDocument(pDocument, pElement);
	}

	return pElement;
}

DrctDbtTx::DrctDbtTx() :
Element(TEXT("DrctDbtTx")),
m_MndtRltdInf()
{

}

xercesc::DOMElement* DrctDbtTx::toDOMDocument(xercesc::DOMDocument *pDocument, xercesc::DOMElement *pParent)
{
	xercesc::DOMElement *pElement = Element::toDOMDocument(pDocument, pParent, true);

	if (pElement)
	{
		m_MndtRltdInf.toDOMDocument(pDocument, pElement);
	}

	return pElement;
}

RmtInf::RmtInf() :
Element(TEXT("RmtInf")),
m_Ustrd(TEXT("Ustrd"))
{

}

xercesc::DOMElement* RmtInf::toDOMDocument(xercesc::DOMDocument *pDocument, xercesc::DOMElement *pParent)
{
	xercesc::DOMElement *pElement = Element::toDOMDocument(pDocument, pParent, true);

	if (pElement)
	{
		m_Ustrd.toDOMDocument(pDocument, pElement);
	}

	return pElement;
}

DrctDbtTxInf::DrctDbtTxInf() :
Element(TEXT("DrctDbtTxInf")),
m_PmtId(),
m_InstdAmt(TEXT("InstdAmt")),
m_DrctDbtTx(),
m_DbtrAgt(TEXT("DbtrAgt")),
m_Dbtr(TEXT("Dbtr")),
m_DbtrAcct(TEXT("DbtrAcct")),
m_RmtInf()
{

}

xercesc::DOMElement* DrctDbtTxInf::toDOMDocument(xercesc::DOMDocument *pDocument, xercesc::DOMElement *pParent)
{
	xercesc::DOMElement *pInstdAmt;
	xercesc::DOMElement *pElement = Element::toDOMDocument(pDocument, pParent, true);

	if (pElement)
	{
		m_PmtId.toDOMDocument(pDocument, pElement);
		pInstdAmt = m_InstdAmt.toDOMDocument(pDocument, pElement);
		if (pInstdAmt)
			pInstdAmt->setAttribute(TEXT("Ccy"), TEXT("EUR"));
		m_DrctDbtTx.toDOMDocument(pDocument, pElement);
		m_DbtrAgt.toDOMDocument(pDocument, pElement);
		m_Dbtr.toDOMDocument(pDocument, pElement);
		m_DbtrAcct.toDOMDocument(pDocument, pElement);
		m_RmtInf.toDOMDocument(pDocument, pElement);
	}

	return pElement;
}

DirectDebitTransactionInformation::DirectDebitTransactionInformation(IUnknown *pParent) :
SepaControlDispatch<IDirectDebitTransactionInformation>(pParent)
{
	m_pDrctDbtTxInf = new DrctDbtTxInf();
	m_bOwnDrctDbtTxInf = true;
}

DirectDebitTransactionInformation::~DirectDebitTransactionInformation()
{
	if (m_bOwnDrctDbtTxInf)
		delete m_pDrctDbtTxInf;
}

void DirectDebitTransactionInformation::Detach()
{
	m_bOwnDrctDbtTxInf = false;
}

DrctDbtTxInf* DirectDebitTransactionInformation::getDrctDbtTxInf() const
{
	return m_pDrctDbtTxInf;
}

STDMETHODIMP DirectDebitTransactionInformation::GetPaymentIdentificationEndToEndId(BSTR *pValue)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;

	*pValue = _bstr_t(m_pDrctDbtTxInf->m_PmtId.m_EndToEndId.getTextValue()).Detach();

	return S_OK;
}

STDMETHODIMP DirectDebitTransactionInformation::SetPaymentIdentificationEndToEndId(BSTR Value)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;

	m_pDrctDbtTxInf->m_PmtId.m_EndToEndId.setValue(Value);

	return S_OK;
}

STDMETHODIMP DirectDebitTransactionInformation::GetInstructedAmount(VARIANT *pValue)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;

	*pValue = _variant_t(m_pDrctDbtTxInf->m_InstdAmt.getDoubleValue()).Detach();

	return S_OK;
}

STDMETHODIMP DirectDebitTransactionInformation::SetInstructedAmount(VARIANT Value)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;


	m_pDrctDbtTxInf->m_InstdAmt.setValue((double)_variant_t(Value));

	return S_OK;
}

STDMETHODIMP DirectDebitTransactionInformation::GetMandateIdentification(BSTR *pValue)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;

	*pValue = _bstr_t(m_pDrctDbtTxInf->m_DrctDbtTx.m_MndtRltdInf.m_MndtId.getTextValue()).Detach();

	return S_OK;
}

STDMETHODIMP DirectDebitTransactionInformation::SetMandateIdentification(BSTR Value)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;

	m_pDrctDbtTxInf->m_DrctDbtTx.m_MndtRltdInf.m_MndtId.setValue(Value);

	return S_OK;
}

STDMETHODIMP DirectDebitTransactionInformation::GetDateOfSignature(DATE *pValue)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;

	return DateTypeFromStdTime(m_pDrctDbtTxInf->m_DrctDbtTx.m_MndtRltdInf.m_DtOfSgntr.getDateValue(), pValue);
}

STDMETHODIMP DirectDebitTransactionInformation::SetDateOfSignature(DATE Value)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;

	m_pDrctDbtTxInf->m_DrctDbtTx.m_MndtRltdInf.m_DtOfSgntr.setValue(StdTimeFromDateType(Value));

	return S_OK;
}

STDMETHODIMP DirectDebitTransactionInformation::GetDebtorAgentFinancialInstitutionIdentificationBIC(BSTR *pValue)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;

	*pValue = _bstr_t(m_pDrctDbtTxInf->m_DbtrAgt.m_FinancialInstitutionIdentification.m_BIC.getTextValue()).Detach();

	return S_OK;
}

STDMETHODIMP DirectDebitTransactionInformation::SetDebtorAgentFinancialInstitutionIdentificationBIC(BSTR Value)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;

	m_pDrctDbtTxInf->m_DbtrAgt.m_FinancialInstitutionIdentification.m_BIC.setValue(Value);

	return S_OK;
}

STDMETHODIMP DirectDebitTransactionInformation::GetDebtorName(BSTR *pValue)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;

	*pValue = _bstr_t(m_pDrctDbtTxInf->m_Dbtr.m_Nm.getTextValue()).Detach();

	return S_OK;
}

STDMETHODIMP DirectDebitTransactionInformation::SetDebtorName(BSTR Value)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;

	m_pDrctDbtTxInf->m_Dbtr.m_Nm.setValue(Value);

	return S_OK;
}

STDMETHODIMP DirectDebitTransactionInformation::GetDebtorAccountIBAN(BSTR *pValue)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;
	
	*pValue = _bstr_t(m_pDrctDbtTxInf->m_DbtrAcct.m_Id.m_IBAN.getTextValue()).Detach();

	return S_OK;
}

STDMETHODIMP DirectDebitTransactionInformation::SetDebtorAccountIBAN(BSTR Value)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;

	m_pDrctDbtTxInf->m_DbtrAcct.m_Id.m_IBAN.setValue(Value);

	return S_OK;
}

STDMETHODIMP DirectDebitTransactionInformation::GetRemittanceInformationUnstructured(BSTR *pValue)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;

	*pValue = _bstr_t(m_pDrctDbtTxInf->m_RmtInf.m_Ustrd.getTextValue()).Detach();

	return S_OK;
}

STDMETHODIMP DirectDebitTransactionInformation::SetRemittanceInformationUnstructured(BSTR Value)
{
	if (!m_pDrctDbtTxInf)
		return E_UNEXPECTED;

	m_pDrctDbtTxInf->m_RmtInf.m_Ustrd.setValue(Value);

	return S_OK;
}
