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
#pragma once

#include "resqml2/AbstractValuesProperty.h"

namespace RESQML2_0_1_NS
{
	class DLL_IMPORT_OR_EXPORT CategoricalProperty : public RESQML2_NS::AbstractValuesProperty
	{
	public:

		/**
		* Only to be used in partial transfer context
		*/
		CategoricalProperty(gsoap_resqml2_0_1::eml20__DataObjectReference* partialObject) : RESQML2_NS::AbstractValuesProperty(partialObject) {}

		CategoricalProperty(StringTableLookup* strLookup): stringLookup(strLookup) {}

		/**
		* Creates an instance of this class which is of a well known Energistics property kind.
		* @param rep						The representation which supports these property values.
		* @param guid						The guid to set to the fault. If empty then a new guid will be generated.
		* @param title						A title for the instance to create.
		* @param dimension					The dimension of each value (scalar properties == 1).
		* @param attachmentKind				The topological orbit which support each value.
		* @param strLookup					The string lookup which defines the possible string values and their keys.
		* @param energisticsPropertyKind	The property kind of these property values which must be defined in the standard energistics property type dictionary.
		*/
		CategoricalProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			const unsigned int & dimension, const gsoap_resqml2_0_1::resqml2__IndexableElements & attachmentKind,
			class StringTableLookup* strLookup, const gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind & energisticsPropertyKind);

		/**
		* Creates an instance of this class which is of a local property kind.
		* @param rep						The representation which supports these property values.
		* @param guid						The guid to set to the fault. If empty then a new guid will be generated.
		* @param title						A title for the instance to create.
		* @param dimension					The dimension of each value (scalar properties == 1).
		* @param attachmentKind				The topological orbit which support each value.
		* @param strLookup					The string lookup which defines the possible string values and their keys.
		* @param localPropKind				The property kind of these property values which must be defined in the EPC document as a local property kind.
		*/
		CategoricalProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			const unsigned int & dimension, const gsoap_resqml2_0_1::resqml2__IndexableElements & attachmentKind,
			class StringTableLookup* strLookup, RESQML2_NS::PropertyKind * localPropKind);

		/**
		* Creates an instance of this class by wrapping a gsoap instance.
		*/
		CategoricalProperty(gsoap_resqml2_0_1::_resqml2__CategoricalProperty* fromGsoap): AbstractValuesProperty(fromGsoap), stringLookup(nullptr) {}

		/**
		* Destructor does nothing since the memory is managed by the gsoap context.
		*/
		virtual ~CategoricalProperty() {}

		static const char* XML_TAG;
		virtual std::string getXmlTag() const {return XML_TAG;}

		/**
		* Add a 1d array of explicit long values to the property values.
		* @param values					All the property values to set ordered according to the topology of the representation it is based on.
		* @param valueCount				The number of values to write.
		* @param proxy					The HDF proxy where to write the property values. It must be already opened for writing and won't be closed in this method.
		*/
		void pushBackLongHdf5Array1dOfValues(long * values, const ULONG64 & valueCount, COMMON_NS::AbstractHdfProxy * proxy, const long & nullValue);

		/**
		* Add a 2d array of explicit long values to the property values.
		* @param values					All the property values to set ordered according to the topology of the representation it is based on.
		* @param valueCountInFastestDim	The number of values to write in the fastest dimension (mainly I dimension).
		* @param valueCountInSlowestDim The number of values to write in the slowest dimension (mainly J dimension).
		* @param proxy					The HDF proxy where to write the property values. It must be already opened for writing and won't be closed in this method.
		*/
		void pushBackLongHdf5Array2dOfValues(long * values, const ULONG64 & valueCountInFastestDim, const ULONG64 & valueCountInSlowestDim, COMMON_NS::AbstractHdfProxy * proxy, const long & nullValue);

		/**
		* Add a 3d array of explicit long values to the property values.
		* @param values					All the property values to set ordered according to the topology of the representation it is based on.
		* @param valueCountInFastestDim	The number of values to write in the fastest dimension (mainly I dimension).
		* @param valueCountInMiddleDim	The number of values to write in the middle dimension (mainly J dimension).
		* @param valueCountInSlowestDim The number of values to write in the slowest dimension (mainly K dimension).
		* @param proxy					The HDF proxy where to write the property values. It must be already opened for writing and won't be closed in this method.
		*/
		void pushBackLongHdf5Array3dOfValues(long * values, const ULONG64 & valueCountInFastestDim, const ULONG64 & valueCountInMiddleDim, const ULONG64 & valueCountInSlowestDim, COMMON_NS::AbstractHdfProxy * proxy, const long & nullValue);

		/**
		* Add an array (potentially multi dimensions) of long values to the property values which will be stored in the HDF file identified by its HDF proxy.
		* @param values					All the property values to set ordered according to the topology of the representation it is based on.
		* @param numValues				The number of property values ordered by dimension of the array to write.
		* @param numDimensionsInArray	The number of dimensions in the array to write.
		* @param proxy					The HDF proxy where to write the property values. It must be already opened for writing and won't be closed in this method.
		*/
		void pushBackLongHdf5ArrayOfValues(long * values, unsigned long long * numValues, const unsigned int & numDimensionsInArray, COMMON_NS::AbstractHdfProxy* proxy, const long & nullValue);

		/**
		* Add a 1d array of explicit unsigned short values to the property values.
		* @param values					All the property values to set ordered according to the topology of the representation it is based on.
		* @param valueCount				The number of values to write.
		* @param proxy					The HDF proxy where to write the property values. It must be already opened for writing and won't be closed in this method.
		*/
		void pushBackUShortHdf5Array1dOfValues(unsigned short * values, const ULONG64 & valueCount, COMMON_NS::AbstractHdfProxy * proxy, const long & nullValue);

		/**
		* Add a 2d array of explicit unsigned short values to the property values.
		* @param values					All the property values to set ordered according to the topology of the representation it is based on.
		* @param valueCountInFastestDim	The number of values to write in the fastest dimension (mainly I dimension).
		* @param valueCountInSlowestDim The number of values to write in the slowest dimension (mainly J dimension).
		* @param proxy					The HDF proxy where to write the property values. It must be already opened for writing and won't be closed in this method.
		*/
		void pushBackUShortHdf5Array2dOfValues(unsigned short * values, const ULONG64 & valueCountInFastestDim, const ULONG64 & valueCountInSlowestDim, COMMON_NS::AbstractHdfProxy * proxy, const long & nullValue);

		/**
		* Add a 3d array of explicit unsigned short values to the property values.
		* @param values					All the property values to set ordered according to the topology of the representation it is based on.
		* @param valueCountInFastestDim	The number of values to write in the fastest dimension (mainly I dimension).
		* @param valueCountInMiddleDim	The number of values to write in the middle dimension (mainly J dimension).
		* @param valueCountInSlowestDim The number of values to write in the slowest dimension (mainly K dimension).
		* @param proxy					The HDF proxy where to write the property values. It must be already opened for writing and won't be closed in this method.
		*/
		void pushBackUShortHdf5Array3dOfValues(unsigned short * values, const ULONG64 & valueCountInFastestDim, const ULONG64 & valueCountInMiddleDim, const ULONG64 & valueCountInSlowestDim, COMMON_NS::AbstractHdfProxy * proxy, const long & nullValue);

		/**
		* Add an array (potentially multi dimensions) of unsigned short values to the property values which will be stored in the HDF file identified by its HDF proxy.
		* @param values					All the property values to set ordered according to the topology of the representation it is based on.
		* @param numValues				The number of property values ordered by dimension of the array to write.
		* @param numDimensionsInArray	The number of dimensions in the array to write.
		* @param proxy					The HDF proxy where to write the property values. It must be already opened for writing and won't be closed in this method.
		*/
		void pushBackUShortHdf5ArrayOfValues(unsigned short * values, unsigned long long * numValues, const unsigned int & numDimensionsInArray, COMMON_NS::AbstractHdfProxy* proxy, const unsigned short & nullValue);

		std::string pushBackRefToExistingDataset(COMMON_NS::AbstractHdfProxy* hdfProxy, const std::string & dataset = "", const long & nullValue = (std::numeric_limits<long>::max)());

		/**
		* Get the string lookup which is associated to this categorical property values.
		*/
		class StringTableLookup* getStringLookup() {return stringLookup;}

		/**
		* Get the string lookup uuid which is associated to this categorical property values.
		*/
		std::string getStringLookupUuid() const;

		/**
		* Check if the associated local property kind is allowed for this property.
		*/
		bool validatePropertyKindAssociation(RESQML2_NS::PropertyKind* pk);

		/**
		* Check if the associated standard property kind is allowed for this property.
		*/
		bool validatePropertyKindAssociation(const gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind & pk);

	protected:
		std::vector<epc::Relationship> getAllEpcRelationships() const;
		virtual void importRelationshipSetFromEpc(COMMON_NS::EpcDocument* epcDoc);

		// XML forward relationship
		class StringTableLookup* stringLookup;
	};
}


