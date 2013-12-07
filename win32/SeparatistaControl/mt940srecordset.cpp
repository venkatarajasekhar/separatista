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

#include <comutil.h>

#include "mt940srecordset.h"
#include "dispatch.cpp"
#include "util.h"
#include "enumvariant.h"

CMT940SRecordset::CMT940SRecordset(IDispatch *pParent)
:SepaControlDispatch<IMT940SRecordset>(pParent)
{
	m_pMT940SRecordset = NULL;
	m_transactionIndex = 0;
}

CMT940SRecordset& CMT940SRecordset::operator = (Separatista::MT940SRecordset *pMT940SRecordset)
{
	m_pMT940SRecordset = pMT940SRecordset;
	
	return *this;
}

STDMETHODIMP CMT940SRecordset::CurrencyClient(BSTR *pCurrencyClient)
{
	if(!m_pMT940SRecordset)
		return E_UNEXPECTED;

	*pCurrencyClient = _bstr_t(m_pMT940SRecordset->getCurrencyClient());
	return S_OK;
}

STDMETHODIMP CMT940SRecordset::TransactionReference(BSTR *pTransactionReference)
{
	if(!m_pMT940SRecordset)
		return E_UNEXPECTED;

	*pTransactionReference = _bstr_t(m_pMT940SRecordset->getTransactionReference());

	return S_OK;
}

STDMETHODIMP CMT940SRecordset::SerialNumber(BSTR *pSerialNumber)
{
	if(!m_pMT940SRecordset)
		return E_UNEXPECTED;

	*pSerialNumber = _bstr_t(m_pMT940SRecordset->getSerialNumber());

	return S_OK;
}

STDMETHODIMP CMT940SRecordset::IBANClient(CIBAN **ppIBANClient)
{
	if(!m_pMT940SRecordset)
		return E_UNEXPECTED;

	*ppIBANClient = new CIBAN();
	if(!*ppIBANClient)
		return E_OUTOFMEMORY;

	(*ppIBANClient)->AddRef();

	(**ppIBANClient) = (Separatista::IBAN*)m_pMT940SRecordset->getIBANClient();

	return S_OK;
}

STDMETHODIMP CMT940SRecordset::PreviousBalance(VARIANT *pPreviousBalance)
{
	HRESULT hr;

	if(!m_pMT940SRecordset)
		return E_UNEXPECTED;

	hr = VariantTypeFromCurrency(m_pMT940SRecordset->getPreviousBalance()->getCurrency(), pPreviousBalance);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

STDMETHODIMP CMT940SRecordset::PreviousBalanceDate(DATE *pPreviousBalanceDate)
{
	time_t t;
	const Separatista::MT940SDate *pDate;

	if(!m_pMT940SRecordset)
		return E_UNEXPECTED;

	// Check for time set in recordset
	pDate = m_pMT940SRecordset->getPreviousBalanceDate();
	if(!pDate)
		t = time(NULL);
	else
		t = pDate->getTime();

	return DateTypeFromStdTime(t, pPreviousBalanceDate);
}

STDMETHODIMP CMT940SRecordset::CurrentBalance(VARIANT *pCurrentBalance)
{
	HRESULT hr;

	if (!m_pMT940SRecordset)
		return E_UNEXPECTED;

	hr = VariantTypeFromCurrency(m_pMT940SRecordset->getCurrentBalance()->getCurrency(), pCurrentBalance);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

STDMETHODIMP CMT940SRecordset::CurrentBalanceDate(DATE *pCurrentBalanceDate)
{
	const Separatista::MT940SDate *pDate;
	time_t t;

	if(!m_pMT940SRecordset)
		return E_UNEXPECTED;

	// Check for time set in recordset
	pDate = m_pMT940SRecordset->getCurrentBalanceDate();
	if(!pDate)
		t = time(NULL);
	else
		t = pDate->getTime();

	return DateTypeFromStdTime(t, pCurrentBalanceDate);
}

STDMETHODIMP CMT940SRecordset::MoveFirst()
{
	if(!m_pMT940SRecordset)
		return E_UNEXPECTED;

	m_transactionIndex = 0;

	return S_OK;
}

STDMETHODIMP CMT940SRecordset::MoveNext()
{
	BOOL b;
	HRESULT hr;

	if(!m_pMT940SRecordset)
		return E_UNEXPECTED;

	hr = FEOF(&b);
	if (b)
		return E_ABORT;
	if (FAILED(hr))
		return hr;

	m_transactionIndex++;

	return S_OK;
}

STDMETHODIMP CMT940SRecordset::FEOF(BOOL *pEOF)
{
	if(!m_pMT940SRecordset)
		return E_UNEXPECTED;

	if (m_transactionIndex <= m_pMT940SRecordset->getTransactionCount())
		*pEOF = false;
	else
		*pEOF = true;

	return S_OK;
}

STDMETHODIMP CMT940SRecordset::TransactionDate(DATE *pDate)
{
	Separatista::MT940STransaction *pTransaction;

	if (!m_pMT940SRecordset)
		return E_UNEXPECTED;

	pTransaction = m_pMT940SRecordset->getTransaction(m_transactionIndex);
	if (!pTransaction)
		return E_ABORT;

	return ::DateTypeFromStdTime((time_t)pTransaction->getDate(), pDate);
}

STDMETHODIMP CMT940SRecordset::RDCCode(BSTR *pRDCCode)
{
	Separatista::MT940STransaction *pTransaction;

	if (!m_pMT940SRecordset)
		return E_UNEXPECTED;

	pTransaction = m_pMT940SRecordset->getTransaction(m_transactionIndex);
	if (!pTransaction)
		return E_ABORT;

	*pRDCCode = _bstr_t(pTransaction->getRDCCode());

	return S_OK;
}

STDMETHODIMP CMT940SRecordset::Currency(VARIANT *pCurrency)
{
	Separatista::MT940STransaction *pTransaction;

	if (!m_pMT940SRecordset)
		return E_UNEXPECTED;

	pTransaction = m_pMT940SRecordset->getTransaction(m_transactionIndex);
	if (!pTransaction)
		return E_ABORT;

	return ::VariantTypeFromCurrency((const char*)pTransaction->getCurrency(), pCurrency);
}

STDMETHODIMP CMT940SRecordset::TransactionCode(BSTR *pTransactionCode)
{
	Separatista::MT940STransaction *pTransaction;

	if (!m_pMT940SRecordset)
		return E_UNEXPECTED;

	pTransaction = m_pMT940SRecordset->getTransaction(m_transactionIndex);
	if (!pTransaction)
		return E_ABORT;

	*pTransactionCode = _bstr_t(pTransaction->getTransactionCode());

	return S_OK;
}

STDMETHODIMP CMT940SRecordset::TransactionRef(BSTR *pTransactionReference)
{
	Separatista::MT940STransaction *pTransaction;

	if (!m_pMT940SRecordset)
		return E_UNEXPECTED;

	pTransaction = m_pMT940SRecordset->getTransaction(m_transactionIndex);
	if (!pTransaction)
		return E_ABORT;

	*pTransactionReference = _bstr_t(pTransaction->getTransactionReference());

	return S_OK;
}

STDMETHODIMP CMT940SRecordset::ForeignIBAN(CIBAN **ppIBAN)
{
	Separatista::MT940STransaction *pTransaction;

	if (!m_pMT940SRecordset)
		return E_UNEXPECTED;

	pTransaction = m_pMT940SRecordset->getTransaction(m_transactionIndex);
	if (!pTransaction)
		return E_ABORT;

	*ppIBAN = new CIBAN(m_pParent);
	if (!*ppIBAN)
		return E_OUTOFMEMORY;

	(*ppIBAN)->AddRef();
	(**ppIBAN) = (Separatista::IBAN*)pTransaction->getForeignIBAN();

	return S_OK;
}

STDMETHODIMP CMT940SRecordset::Description(VARIANT vKey, BSTR *pValue)
{
	Separatista::MT940STransaction *pTransaction;
	const char *pv;
	_variant_t vk(vKey);

	if (!m_pMT940SRecordset)
		return E_UNEXPECTED;

	pTransaction = m_pMT940SRecordset->getTransaction(m_transactionIndex);
	if (!pTransaction)
		return E_ABORT;

	pv = pTransaction->getDescription(_bstr_t(vk));

	*pValue = _bstr_t(pv);

	return S_OK;
}

STDMETHODIMP CMT940SRecordset::_NewEnum(IUnknown **ppUnk)
{
	Separatista::MT940STransaction *pTransaction;
	EnumVariant *pEnumVariant;

	if (!m_pMT940SRecordset)
		return E_UNEXPECTED;

	pTransaction = m_pMT940SRecordset->getTransaction(m_transactionIndex);
	if (!pTransaction)
		return E_ABORT;

	// Create new EnumVariant
	pEnumVariant = new EnumVariant();
	if (!pEnumVariant)
		return E_OUTOFMEMORY;
	pEnumVariant->AddRef();

	// Enum all descriptions in this transaction
	for (pTransaction->moveFirstDescription(); !pTransaction->descriptionEOF(); pTransaction->moveNextDescription())
	{
		// Add description key to EnumVariant
		pEnumVariant->Add(_variant_t(pTransaction->getDescriptionKey()));
	}

	// Set out
	*ppUnk = pEnumVariant;

	return S_OK;
}
