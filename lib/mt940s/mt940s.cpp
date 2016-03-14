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

#include <istream>
#include <fstream>
#include <string>

#include "mt940s.h"
#include "separatista/iban/iban.h"

using namespace std;
using namespace Separatista;

MT940SDocument::MT940SDocument()
{
}

MT940SDocument::~MT940SDocument()
{
	vector<MT940SRecordset*>::iterator it;

	// Delete all recordsets
	for(it = m_recordsets.begin(); it != m_recordsets.end(); it++)
		try{
		delete *it;
		}catch (...){
                 cout << "Error, leaving function now";
                 }

size_t MT940SDocument::getRecordsetCount()
{
	return m_recordsets.size();
}

MT940SRecordset* MT940SDocument::getRecordset(size_t i) 
{
	if (i >= m_recordsets.size())
		return NULL;
	return m_recordsets[i];
}

MT940SRecordset::~MT940SRecordset()
{
	vector<MT940STransaction*>::iterator it;

	// Delete all transactions
	for(it = m_transactions.begin(); it != m_transactions.end(); it++)
			try{
		delete *it;
		}catch (...){
                 cout << "Error, leaving function now";
                 }

}

MT940SDocument::OpenStatus MT940SDocument::Open(const char *path)
{
	ifstream ifs;
	string line, header;
	string::iterator it;
	MT940SRecordset *recordset;
	int nline, rline;
	MT940SDocument::OpenStatus status = OK;

	// Open file
	ifs.open(path);
	if(!ifs.is_open())
		return MT940SDocument::E_FILE;

	// Read lines from file
	nline = 0, rline = 0;
	recordset = NULL;
	while (ifs.good() && status == OK)
	{
		// Read line
		getline(ifs, line);
		
		// Check for first :
		if(line.length() != 0)
		{
			it = line.begin();
			if(*it == ':')
			{
				// It's a new record, get the code
				rline = 0;
				header.clear();
				for(++it; it != line.end() && *it != ':'; ++it)
					header += *it;

				if(header == "940")
				{
					recordset = NULL;
					status = ReadRecord940(nline, rline);
				}
				else
				{
					// Recordset should start with record 20
					if(header == "20")
					{
						recordset = new MT940SRecordset();
						if (!recordset)
							return E_MEMORY;
						m_recordsets.push_back(recordset);
					}
				}

				// Next character
				++it;
			} else {
				// It's the same record
				rline++;
			}

			// Check recordset
			if(recordset != NULL)
				status = recordset->ReadRecord(nline, rline, header, string(it, line.end()));
		}
		nline++;
	}

	ifs.close();
	return status;
}

MT940SDocument::OpenStatus MT940SDocument::ReadRecord940(int line, int rline)
{
	return MT940SDocument::OK;
}

MT940SRecordset::MT940SRecordset()
{
}

MT940SDocument::OpenStatus MT940SRecordset::ReadRecord(int line, int rline, std::string &header, std::string &data)
{
	try{
		// Transaction Reference record
		if (header == "20")
			return ReadRecord20(line, rline, data);

		// Account number record
		if (header == "25")
			return ReadRecord25(line, rline, data);

		// Serial number record
		if (header == "28C")
			return ReadRecord28C(line, rline, data);

		// Previous balance record
		if (header == "60F")
			return ReadRecord60F(line, rline, data);

		// Transcript record
		if (header == "61")
			return ReadRecord61(line, rline, data);

		// Description record
		if (header == "86")
			return ReadRecord86(line, rline, data);

		// Current balance record
		if (header == "62F")
			return ReadRecord62F(line, rline, data);
	}
	catch (exception& e)
	{
		return MT940SDocument::E_FORMAT;
	}

	return MT940SDocument::OK;
}

MT940SDocument::OpenStatus MT940SRecordset::ReadRecord20(int line, int rline, std::string &data)
{
	m_transactionReference = data;
	return MT940SDocument::OK;
}

MT940SDocument::OpenStatus MT940SRecordset::ReadRecord25(int line, int rline, std::string &data)
{
	size_t i;

	// Account number and currency seperated by space
	i = data.find(' ');
	if(i == string::npos)
		return MT940SDocument::E_FORMAT;
	m_ibanClient = data.substr(0, i).c_str();
	m_currencyClient = data.substr(i + 1);
	return MT940SDocument::OK;
}

MT940SDocument::OpenStatus MT940SRecordset::ReadRecord28C(int line, int rline, std::string &data)
{
	m_serialNumber = data;
	return MT940SDocument::OK;
}

MT940SDocument::OpenStatus MT940SRecordset::ReadRecord60F(int line, int rline, std::string &data)
{
	// Get the date first
	m_previousBalanceDate = data.substr(1, 6).c_str();
	// Get the balance
	m_previousBalance.set(data[0], data.substr(7, 3).c_str(), data.substr(10).c_str());
	return MT940SDocument::OK;
}

MT940SDocument::OpenStatus MT940SRecordset::ReadRecord61(int line, int rline, std::string &data)
{
	size_t i;
	string code;
	MT940STransaction *pTransaction;

	// Check record line
	if(rline == 0)
	{
		// Create new transaction and push it to the vector
	        try{
		pTransaction = new MT940STransaction;
	        }catch(...){
	        	
	        }
		if (!pTransaction)
			return MT940SDocument::E_MEMORY;
		m_transactions.push_back(pTransaction);

		// Get the transaction date
		pTransaction->setDate(data.substr(0, 6).c_str());

		// rdc code
		if(data[6] == 'R')
		{
			// RD or RC
			i = 1;
			code = "R";
			code += data[7];
		} 
		else 
		{
			i = 0;
			code = data[6];
		}
		pTransaction->setRDCCode(code.c_str());

		// Currency
		pTransaction->setCurrency(data[6 + i], m_currencyClient.c_str(), data.substr(7 + i, 15).c_str());

		// Transaction code
		pTransaction->setTransactionCode(data.substr(22 + i, 4).c_str());

		// Transaction reference
		pTransaction->setTransactionReference(data.substr(26 + i, 16).c_str());
	}
	else
	{
		// Check transaction
		if(m_transactions.empty())
			return MT940SDocument::E_FORMAT;
		pTransaction = m_transactions.back();

		// Get current record
		pTransaction->setForeignIBAN(data.c_str());
	}

	return MT940SDocument::OK;
}

MT940SDocument::OpenStatus MT940SRecordset::ReadRecord86(int line, int rline, std::string &data)
{
	MT940STransaction *pTransaction;
	string key, value;
	const char* const keys[] = {
		"MARF",
		"EREF",
		"PREF",
		"RTRN",
		"ACCW",
		"BENM",
		"NAME",
		"ORDP",
		"ID",
		"ADDR",
		"REMI",
		"ISDT",
		"CSID"
	};

	// Get current transaction
	if(m_transactions.empty())
		return MT940SDocument::E_FORMAT;
	pTransaction = m_transactions.back();
	size_t offset, end;

	// Clear description on init
	if (rline == 0)
		m_description.clear();

	// Append data to description
	m_description += data;

	// Keep checking offset for out-of-bounds or trailing '/'
	// 
	offset = 0;
	bool found = false;
loop_while:
	while (offset < m_description.length() && m_description[offset++] == '/')
	{
		// Find key
		for (int i = 0; !found && i < (sizeof(keys) / sizeof(*keys)); i++)
		{
			// Check strlen characters
			if (m_description.compare(offset, strlen(keys[i]), keys[i]) == 0)
			{
				// Match
				found = true;
				key = keys[i];
				offset += strlen(keys[i]);
				value.clear(); // Important
				goto loop_while; // Recheck while conditions
			}
		}

		// Find end as count from offset to next '/' or to end of line
		end = m_description.find('/', offset);
		if (end == string::npos)
			end = m_description.length() - 1;

		// Check found
		if (!found)
		{
			// If the key wasn't found it's most probably because of a '/' character in the description value			
			// And replace the value in the map
			value += m_description.substr(offset, end - offset);
		}
		else
		{
			// If the key was found add it to the maps
			value = m_description.substr(offset, end - offset);
		}

		pTransaction->addDescription(key.c_str(), value.c_str());
		offset = end;
		found = false;
	}

	return MT940SDocument::OK;
}

MT940SDocument::OpenStatus MT940SRecordset::ReadRecord62F(int line, int rline, std::string &data)
{
	// Get the date first
	m_currentBalanceDate = data.substr(1, 6).c_str();
	// Get the balance
	m_currentBalance.set(data[0], data.substr(7, 3).c_str(), data.substr(10).c_str());

	return MT940SDocument::OK;
}

const char* MT940SRecordset::getCurrencyClient() const
{
	return m_currencyClient.c_str();
}

const char* MT940SRecordset::getTransactionReference() const
{
	return m_transactionReference.c_str();
}

const char* MT940SRecordset::getSerialNumber() const
{
	return m_serialNumber.c_str();
}

const IBAN* MT940SRecordset::getIBANClient() const
{
	return &m_ibanClient;
}

MT940SCurrency* MT940SRecordset::getPreviousBalance()
{
	return &m_previousBalance;
}

const MT940SDate* MT940SRecordset::getPreviousBalanceDate() const
{
	return &m_previousBalanceDate;
}

MT940SCurrency* MT940SRecordset::getCurrentBalance()
{
	return &m_currentBalance;
}

const MT940SDate* MT940SRecordset::getCurrentBalanceDate() const
{
	return &m_currentBalanceDate;
}

size_t MT940SRecordset::getTransactionCount() const
{
	return m_transactions.size();
}

MT940STransaction* MT940SRecordset::getTransaction(size_t index)
{
	if (index >= m_transactions.size())
		return NULL;

	return m_transactions.at(index);
}

MT940STransaction::MT940STransaction()
{
}

MT940STransaction::~MT940STransaction()
{
}

MT940SDate* MT940STransaction::getDate()
{
	return &m_date;
}

void MT940STransaction::setDate(const char *date)
{
	m_date = date;
}

const char* MT940STransaction::getRDCCode() const
{
	return m_rdcCode.c_str();
}

void MT940STransaction::setRDCCode(const char *code)
{
	m_rdcCode = code;
}

MT940SCurrency* MT940STransaction::getCurrency()
{
	return &m_currency;
}

void MT940STransaction::setCurrency(const char dcCode, const char *currency, const char *amount)
{
	m_currency.set(dcCode, currency, amount);
}

const char* MT940STransaction::getTransactionCode() const
{
	return m_transactionCode.c_str();
}

void MT940STransaction::setTransactionCode(const char *code)
{
	m_transactionCode = code;
}

const char* MT940STransaction::getTransactionReference() const
{
	return m_transactionReference.c_str();
}

void MT940STransaction::setTransactionReference(const char *code)
{
	m_transactionReference = code;
}

const IBAN* MT940STransaction::getForeignIBAN() const
{
	return &m_foreignIBAN;
}

void MT940STransaction::setForeignIBAN(const char *iban)
{
	m_foreignIBAN = iban;
}

const char* MT940STransaction::getDescription(const char *key)
{
	return m_descriptionMap[key].c_str();
}

void MT940STransaction::addDescription(const char *key, const char *value)
{
	m_descriptionMap[key] = value;
	moveFirstDescription();
}

void MT940STransaction::moveFirstDescription()
{
	m_descriptionIterator = m_descriptionMap.begin();
}

bool MT940STransaction::moveNextDescription()
{
	m_descriptionIterator++;

	return descriptionEOF();
}

bool MT940STransaction::descriptionEOF() const
{
	if (m_descriptionIterator == m_descriptionMap.end())
		return true;

	return false;
}

const char* MT940STransaction::getDescriptionKey() const
{
	if (descriptionEOF())
		return NULL;

	return (*m_descriptionIterator).first.c_str();
}

const char* MT940STransaction::getDescriptionValue() const
{
	if (descriptionEOF())
		return NULL;

	return (*m_descriptionIterator).second.c_str();
}

MT940SCurrency::MT940SCurrency()
{
}

void MT940SCurrency::set(const char dcCode, const char *currency, const char *amount)
{
	m_dcCode = dcCode;
	m_currency = currency;
	m_amount = amount;
}

const char* MT940SCurrency::getCurrency()
{
	// dcCode
	if (m_dcCode == 'D' || m_dcCode == 'd')
		m_buffer = "-";
	else
		m_buffer.clear();

	// Value
	m_buffer += m_amount;

	return m_buffer.c_str();
}

MT940SCurrency::operator const char* ()
{
	return getCurrency();
}

MT940SDate::MT940SDate()
{
}

MT940SDate& MT940SDate::operator =(const char *date)
{
	m_date = date;

	return *this;
}

time_t MT940SDate::getTime() const 
{
	tm timeinfo;
	tm *ptm;
	time_t t;

	// Get the current time for tm_year
	t = time(NULL);
	ptm = localtime(&t);

	//Create the tm
	timeinfo.tm_sec = 0;
	timeinfo.tm_min = 0;
	timeinfo.tm_hour = 0;
	timeinfo.tm_mday = stoi(m_date.substr(4, 2));		// Day of the month
	timeinfo.tm_mon = stoi(m_date.substr(2,2)) - 1;		// Months since jan
	timeinfo.tm_year = ptm->tm_year - (ptm->tm_year % 100) + stoi(m_date.substr(0, 2));	// YY converted to this century
	
	// Convert to time_t
	return mktime(&timeinfo); 
}

MT940SDate::operator time_t() const
{
	return getTime();
}


