/*-----------------------------------------------------------------------
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"; you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
-----------------------------------------------------------------------*/
#include "resqml2_0_1/PropertyKindMapper.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <dirent.h>
#include <sys/types.h>
#endif
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "resqml2_0_1/PropertyKind.h"

using namespace std;
using namespace RESQML2_0_1_NS;

string PropertyKindMapper::loadMappingFilesFromDirectory(const string & directory)
{
#if defined(_WIN32)
	WIN32_FIND_DATA search_data;
	HANDLE handle = FindFirstFile((directory+"\\*").c_str(), &search_data);

	vector<string> files;
	if (handle == INVALID_HANDLE_VALUE){
		throw invalid_argument("Cannot open the directory where the property kind mapper is located : " + directory);
	}

	do {
		string fileName(search_data.cFileName);
#else
	DIR* rep = opendir(directory.c_str());
	if (rep == nullptr) {
		throw invalid_argument("Cannot open the directory where the property kind mapper is located : " + directory);
	}

	struct dirent* currentFile = readdir(rep); // first file
	do {
		string fileName = currentFile->d_name;
#endif
		unsigned int pos = fileName.find_last_of('.');
		if (pos < fileName.size()) {
			if (fileName.substr(pos).compare(".xml") == 0 && fileName.find("PropertyKind") != std::string::npos) {
				if (fileName.compare("PropertyKindMapping.xml") == 0) {
					std::ifstream file( (directory + "/PropertyKindMapping.xml").c_str() );

					if ( file ) {
						epcDocument->getGsoapContext()->is = &file;
						gsoap_resqml2_0_1::_ptm__standardEnergisticsPropertyTypeSet* read = gsoap_resqml2_0_1::soap_new_ptm__standardEnergisticsPropertyTypeSet(epcDocument->getGsoapContext(), 1);
						soap_read_ptm__standardEnergisticsPropertyTypeSet(epcDocument->getGsoapContext(), read);
						file.close();

						if (epcDocument->getGsoapContext()->error == SOAP_OK) {
							for (size_t propIndex = 0; propIndex < read->standardEnergisticsPropertyType.size(); ++propIndex)
							{
								resqmlStandardPropertyKindNameToApplicationPropertyKindName[read->standardEnergisticsPropertyType[propIndex]->name] = read->standardEnergisticsPropertyType[propIndex];
								for (size_t equivalentPropIndex = 0; equivalentPropIndex < read->standardEnergisticsPropertyType[propIndex]->equivalentPropertyType.size(); ++equivalentPropIndex)
								{
									string namingSystem = read->standardEnergisticsPropertyType[propIndex]->equivalentPropertyType[equivalentPropIndex]->namingSystem;
									applicationPropertyKindNameToResqmlStandardPropertyKindName[namingSystem][read->standardEnergisticsPropertyType[propIndex]->equivalentPropertyType[equivalentPropIndex]->name] = read->standardEnergisticsPropertyType[propIndex]->name;
								}
							}
						}
						else {
							ostringstream oss;
							soap_stream_fault(epcDocument->getGsoapContext(), oss);
							return oss.str() + " in PropertyKindMapping.xml";
						}
					}
					else {
						throw domain_error("The PropertyKindMapping.xml file cannot be opened");
					}
				}
				else {
					std::ifstream file( (directory + "/" + fileName).c_str() );

					if ( file ) {
						epcDocument->getGsoapContext()->is = &file;
						gsoap_resqml2_0_1::_resqml2__PropertyKind* read = gsoap_resqml2_0_1::soap_new_resqml2__obj_USCOREPropertyKind(epcDocument->getGsoapContext(), 1);
						soap_read_resqml2__obj_USCOREPropertyKind(epcDocument->getGsoapContext(), read);
						file.close();

						if (epcDocument->getGsoapContext()->error == SOAP_OK) {
							resqmlLocalPropertyKindUuidToResqmlLocalPropertyKind[read->uuid] = read;
							for (size_t aliasIndex = 0; aliasIndex < read->Aliases.size(); ++aliasIndex) {
								if (read->Aliases[aliasIndex]->authority) {
									string namingSystem = *read->Aliases[aliasIndex]->authority;
									resqmlLocalPropertyKindUuidToApplicationPropertyKindName[namingSystem][read->uuid] = read->Aliases[aliasIndex]->Identifier;
									applicationPropertyKindNameToResqmlLocalPropertyKindUuid[namingSystem][read->Aliases[aliasIndex]->Identifier] = read->uuid;
								}
							}
						}
						else {
							ostringstream oss;
							soap_stream_fault(epcDocument->getGsoapContext(), oss);
							return oss.str() + " in " + fileName;
						}
					}
					else {
						throw domain_error("The property type config file " + directory + "/" + fileName + " cannot be opened.");
					}
				}
			}
		}
	}
#if defined(_WIN32)
	while (FindNextFile(handle, &search_data) != 0);

	//Close the handle after use or memory/resource leak
	FindClose(handle);
	return string();
#else
	while ((currentFile = readdir(rep)) != nullptr);

	if (closedir(rep) == -1) {
		throw invalid_argument("Cannot close the directory.");
	}

	return string();
#endif
}

std::string PropertyKindMapper::getDescriptionOfResqmlStandardPropertyKindName(const gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind & resqmlStandardPropertyKindName) const
{
#if (defined(_WIN32) && _MSC_VER >= 1600)
	std::unordered_map<gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind, gsoap_resqml2_0_1::ptm__standardEnergisticsPropertyType*>::const_iterator cit = resqmlStandardPropertyKindNameToApplicationPropertyKindName.find (resqmlStandardPropertyKindName);
#elif defined(__APPLE__)
	std::unordered_map<gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind, gsoap_resqml2_0_1::ptm__standardEnergisticsPropertyType*, std::hash<int> >::const_iterator cit = resqmlStandardPropertyKindNameToApplicationPropertyKindName.find (resqmlStandardPropertyKindName);
#else
	std::tr1::unordered_map<gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind, gsoap_resqml2_0_1::ptm__standardEnergisticsPropertyType*, std::tr1::hash<int> >::const_iterator cit = resqmlStandardPropertyKindNameToApplicationPropertyKindName.find (resqmlStandardPropertyKindName);
#endif

	if (cit != resqmlStandardPropertyKindNameToApplicationPropertyKindName.end())
	{
		if (cit->second->description)
			return *(cit->second->description);
	}

	return "";
}

std::string PropertyKindMapper::getApplicationPropertyKindNameFromResqmlStandardPropertyKindName(const gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind & resqmlStandardPropertyKindName, const std::string & application) const
{
#if (defined(_WIN32) && _MSC_VER >= 1600)
	std::unordered_map<gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind, gsoap_resqml2_0_1::ptm__standardEnergisticsPropertyType*>::const_iterator cit = resqmlStandardPropertyKindNameToApplicationPropertyKindName.find (resqmlStandardPropertyKindName);
#elif defined(__APPLE__)
	std::unordered_map<gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind, gsoap_resqml2_0_1::ptm__standardEnergisticsPropertyType*, std::hash<int> >::const_iterator cit = resqmlStandardPropertyKindNameToApplicationPropertyKindName.find (resqmlStandardPropertyKindName);
#else
	std::tr1::unordered_map<gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind, gsoap_resqml2_0_1::ptm__standardEnergisticsPropertyType*, std::tr1::hash<int> >::const_iterator cit = resqmlStandardPropertyKindNameToApplicationPropertyKindName.find (resqmlStandardPropertyKindName);
#endif
	if (cit != resqmlStandardPropertyKindNameToApplicationPropertyKindName.end())
	{
		for (unsigned int equivalentPropIndex = 0; equivalentPropIndex < cit->second->equivalentPropertyType.size(); ++equivalentPropIndex)
		{
			if (cit->second->equivalentPropertyType[equivalentPropIndex]->namingSystem.compare(application) == 0)
			{
				return cit->second->equivalentPropertyType[equivalentPropIndex]->name;
			}
		}
	}

	return "";
}

gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind PropertyKindMapper::getResqmlStandardPropertyKindNameFromApplicationPropertyKindName(const std::string & applicationPropertyKindName, const std::string & application) const
{
#if (defined(_WIN32) && _MSC_VER >= 1600) || defined(__APPLE__)
	std::unordered_map<std::string, std::unordered_map<std::string, gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind> >::const_iterator cit1 = applicationPropertyKindNameToResqmlStandardPropertyKindName.find (application);
	std::unordered_map<std::string, gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind> value;
#else
	std::tr1::unordered_map<std::string, std::tr1::unordered_map<std::string, gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind> >::const_iterator cit1 = applicationPropertyKindNameToResqmlStandardPropertyKindName.find (application);;
	std::tr1::unordered_map<std::string, gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind> value;
#endif
	if (cit1 != applicationPropertyKindNameToResqmlStandardPropertyKindName.end())
		value =  cit1->second;
	else
		return gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind__RESQML_x0020root_x0020property;


#if (defined(_WIN32) && _MSC_VER >= 1600) || defined(__APPLE__)
	std::unordered_map<std::string, gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind>::const_iterator cit = value.find (applicationPropertyKindName);
#else
	std::tr1::unordered_map<std::string, gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind>::const_iterator cit = value.find (applicationPropertyKindName);
#endif
	if (cit != value.end())
		return cit->second;
	else
		return gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind__RESQML_x0020root_x0020property;
}

std::string PropertyKindMapper::getApplicationPropertyKindNameFromResqmlLocalPropertyKindUuid(const std::string & resqmlLocalPropertyKindUuid, const std::string & application) const
{
#if (defined(_WIN32) && _MSC_VER >= 1600) || defined(__APPLE__)
	std::unordered_map<std::string, std::unordered_map<std::string, std::string> >::const_iterator cit1 = resqmlLocalPropertyKindUuidToApplicationPropertyKindName.find (application);
	std::unordered_map<std::string, std::string> value;
#else
	std::tr1::unordered_map< std::string, std::tr1::unordered_map<std::string, std::string> >::const_iterator cit1 = resqmlLocalPropertyKindUuidToApplicationPropertyKindName.find (application);
	std::tr1::unordered_map<std::string, std::string> value;
#endif
	if (cit1 != resqmlLocalPropertyKindUuidToApplicationPropertyKindName.end())
		value =  cit1->second;
	else
		return "";

#if (defined(_WIN32) && _MSC_VER >= 1600) || defined(__APPLE__)
	std::unordered_map<std::string, std::string>::const_iterator cit = value.find (resqmlLocalPropertyKindUuid);
#else
	std::tr1::unordered_map<std::string, std::string>::const_iterator cit = value.find (resqmlLocalPropertyKindUuid);
#endif
	if (cit != value.end())
		return cit->second;
	else
	{
#if (defined(_WIN32) && _MSC_VER >= 1600) || defined(__APPLE__)
		std::unordered_map<std::string, gsoap_resqml2_0_1::_resqml2__PropertyKind*>::const_iterator cit2 = resqmlLocalPropertyKindUuidToResqmlLocalPropertyKind.find(resqmlLocalPropertyKindUuid);
#else
		std::tr1::unordered_map<std::string, gsoap_resqml2_0_1::_resqml2__PropertyKind*>::const_iterator cit2 = resqmlLocalPropertyKindUuidToResqmlLocalPropertyKind.find(resqmlLocalPropertyKindUuid);
#endif
		if (cit2 != resqmlLocalPropertyKindUuidToResqmlLocalPropertyKind.end())
		{
			if (cit2->second->ParentPropertyKind->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml2__LocalPropertyKind)
			{
				return getApplicationPropertyKindNameFromResqmlLocalPropertyKindUuid(static_cast<gsoap_resqml2_0_1::resqml2__LocalPropertyKind*>(cit2->second->ParentPropertyKind)->LocalPropertyKind->UUID, application);
			}
			else
			{
				return getApplicationPropertyKindNameFromResqmlStandardPropertyKindName(static_cast<gsoap_resqml2_0_1::resqml2__StandardPropertyKind*>(cit2->second->ParentPropertyKind)->Kind, application);
			}
		}
	}

	return "";
}

std::string PropertyKindMapper::getResqmlLocalPropertyKindUuidFromApplicationPropertyKindName(const std::string & applicationPropertyKindName, const std::string & application) const
{
#if (defined(_WIN32) && _MSC_VER >= 1600) || defined(__APPLE__)
	std::unordered_map<std::string, std::unordered_map<std::string, std::string> >::const_iterator cit1 = applicationPropertyKindNameToResqmlLocalPropertyKindUuid.find (application);
	std::unordered_map<std::string, std::string> value;
#else
	std::tr1::unordered_map< std::string, std::tr1::unordered_map<std::string, std::string> >::const_iterator cit1 = applicationPropertyKindNameToResqmlLocalPropertyKindUuid.find (application);
	std::tr1::unordered_map<std::string, std::string> value;
#endif
	if (cit1 != applicationPropertyKindNameToResqmlLocalPropertyKindUuid.end())
		value =  cit1->second;
	else
		return "";

#if (defined(_WIN32) && _MSC_VER >= 1600) || defined(__APPLE__)
	std::unordered_map<std::string, std::string>::const_iterator cit = value.find (applicationPropertyKindName);
#else
	std::tr1::unordered_map<std::string, std::string>::const_iterator cit = value.find (applicationPropertyKindName);
#endif
	if (cit != value.end())
		return cit->second;
	else
		return "";
}

PropertyKind* PropertyKindMapper::addResqmlLocalPropertyKindToEpcDocumentFromApplicationPropertyKindName(const std::string & applicationPropertyKindName, const std::string & application)
{
#if (defined(_WIN32) && _MSC_VER >= 1600) || defined(__APPLE__)
	std::unordered_map<std::string, std::unordered_map<std::string, std::string> >::const_iterator cit1 = applicationPropertyKindNameToResqmlLocalPropertyKindUuid.find (application);
	std::unordered_map<std::string, std::string> value;
#else
	std::tr1::unordered_map< std::string, std::tr1::unordered_map<std::string, std::string> >::const_iterator cit1 = applicationPropertyKindNameToResqmlLocalPropertyKindUuid.find (application);
	std::tr1::unordered_map<std::string, std::string> value;
#endif
	if (cit1 != applicationPropertyKindNameToResqmlLocalPropertyKindUuid.end())
		value =  cit1->second;
	else
		return nullptr;

#if (defined(_WIN32) && _MSC_VER >= 1600) || defined(__APPLE__)
	std::unordered_map<std::string, std::string>::const_iterator cit = value.find (applicationPropertyKindName);
#else
	std::tr1::unordered_map<std::string, std::string>::const_iterator cit = value.find (applicationPropertyKindName);
#endif
	if (cit != value.end())
	{
		if (epcDocument->getResqmlAbstractObjectByUuid(cit->second) == nullptr)
		{
			gsoap_resqml2_0_1::_resqml2__PropertyKind* propType = resqmlLocalPropertyKindUuidToResqmlLocalPropertyKind[cit->second];

			std::vector<gsoap_resqml2_0_1::_resqml2__PropertyKind*> toAdd;
			toAdd.push_back(propType);

			while (propType->ParentPropertyKind->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml2__LocalPropertyKind)
			{
				if (epcDocument->getResqmlAbstractObjectByUuid(static_cast<gsoap_resqml2_0_1::resqml2__LocalPropertyKind*>(propType->ParentPropertyKind)->LocalPropertyKind->UUID) == nullptr)
				{
					propType = resqmlLocalPropertyKindUuidToResqmlLocalPropertyKind[static_cast<gsoap_resqml2_0_1::resqml2__LocalPropertyKind*>(propType->ParentPropertyKind)->LocalPropertyKind->UUID];
					toAdd.push_back(propType);
				}
				else
					break;
			}

			std::vector<PropertyKind*> added;
			for (unsigned int toAddIndex = 0; toAddIndex < toAdd.size(); toAddIndex++)
			{
				PropertyKind* wrappedPropType = new PropertyKind(toAdd[toAddIndex]);
				epcDocument->addGsoapProxy(wrappedPropType);
				added.push_back(wrappedPropType);
			}
			for (unsigned int addedIndex = 0; addedIndex < added.size(); addedIndex++)
			{
				added[addedIndex]->importRelationshipSetFromEpc(epcDocument);
			}
			return added[0];
		}
	}

	return nullptr;
}

gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind PropertyKindMapper::getPropertyKindParentOfResqmlStandardPropertyKindName(const gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind & resqmlStandardPropertyKindName) const
{
#if (defined(_WIN32) && _MSC_VER >= 1600)
	std::unordered_map<gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind, gsoap_resqml2_0_1::ptm__standardEnergisticsPropertyType*>::const_iterator cit = resqmlStandardPropertyKindNameToApplicationPropertyKindName.find (resqmlStandardPropertyKindName);
#elif defined(__APPLE__)
	std::unordered_map<gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind, gsoap_resqml2_0_1::ptm__standardEnergisticsPropertyType*, std::hash<int> >::const_iterator cit = resqmlStandardPropertyKindNameToApplicationPropertyKindName.find (resqmlStandardPropertyKindName);
#else
	std::tr1::unordered_map<gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind, gsoap_resqml2_0_1::ptm__standardEnergisticsPropertyType*, std::tr1::hash<int> >::const_iterator cit = resqmlStandardPropertyKindNameToApplicationPropertyKindName.find (resqmlStandardPropertyKindName);
#endif

	if (cit != resqmlStandardPropertyKindNameToApplicationPropertyKindName.end()) {
		if (cit->second->parentKind != nullptr) {
			return *(cit->second->parentKind);
		}
	}

	return gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind__RESQML_x0020root_x0020property;
}

bool PropertyKindMapper::isChildOf(gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind child, gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind parent) const
{
	if (child == gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind__RESQML_x0020root_x0020property) {
		return false;
	}

	if (child == parent) {
		return true;
	}

	return isChildOf(getPropertyKindParentOfResqmlStandardPropertyKindName(child), parent);
}

bool PropertyKindMapper::isAbstract(gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind resqmlStandardPropertyKindName) const
{
#if (defined(_WIN32) && _MSC_VER >= 1600)
	std::unordered_map<gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind, gsoap_resqml2_0_1::ptm__standardEnergisticsPropertyType*>::const_iterator cit = resqmlStandardPropertyKindNameToApplicationPropertyKindName.find(resqmlStandardPropertyKindName);
#elif defined(__APPLE__)
	std::unordered_map<gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind, gsoap_resqml2_0_1::ptm__standardEnergisticsPropertyType*, std::hash<int> >::const_iterator cit = resqmlStandardPropertyKindNameToApplicationPropertyKindName.find(resqmlStandardPropertyKindName);
#else
	std::tr1::unordered_map<gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind, gsoap_resqml2_0_1::ptm__standardEnergisticsPropertyType*, std::tr1::hash<int> >::const_iterator cit = resqmlStandardPropertyKindNameToApplicationPropertyKindName.find(resqmlStandardPropertyKindName);
#endif

	if (cit != resqmlStandardPropertyKindNameToApplicationPropertyKindName.end()) {
		if (cit->second->isAbstract != nullptr) {
			return *(cit->second->isAbstract);
		}
		else {
			return false;
		}
	}

	return true;
}

