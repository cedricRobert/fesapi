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
#include "ContinuousPropertyOnWellFrameTest.h"

#include "catch.hpp"
#include "resqml2_0_1test/WellboreFrameRepresentationTest.h"

#include "resqml2_0_1/ContinuousProperty.h"
#include "resqml2_0_1/WellboreFrameRepresentation.h"
#include "common/AbstractHdfProxy.h"

using namespace std;
using namespace COMMON_NS;
using namespace RESQML2_NS;
using namespace resqml2_0_1test;

const char* ContinuousPropertyOnWellFrameTest::defaultUuid = "6e4ca2c9-6780-44bd-b4c2-6bb3ef4682d4";
const char* ContinuousPropertyOnWellFrameTest::defaultTitle = "Continuous Property on well frame Test";

ContinuousPropertyOnWellFrameTest::ContinuousPropertyOnWellFrameTest(const string & epcDocPath)
	: ContinuousPropertyTest(epcDocPath, defaultUuid, defaultTitle, WellboreFrameRepresentationTest::defaultUuid, WellboreFrameRepresentationTest::defaultTitle) {
}

ContinuousPropertyOnWellFrameTest::ContinuousPropertyOnWellFrameTest(EpcDocument * epcDoc, bool init)
	: ContinuousPropertyTest(epcDoc, defaultUuid, defaultTitle, WellboreFrameRepresentationTest::defaultUuid, WellboreFrameRepresentationTest::defaultTitle) {
	if (init)
		this->initEpcDoc();
	else
		this->readEpcDoc();
}

void ContinuousPropertyOnWellFrameTest::initEpcDocHandler() {
	// creating an IJK grid
	WellboreFrameRepresentationTest * frameTest = new WellboreFrameRepresentationTest(this->epcDoc, true);
	RESQML2_0_1_NS::WellboreFrameRepresentation * frame = static_cast<RESQML2_0_1_NS::WellboreFrameRepresentation *>(this->epcDoc->getResqmlAbstractObjectByUuid(WellboreFrameRepresentationTest::defaultUuid));

	// getting the hdf proxy
	AbstractHdfProxy* hdfProxy = this->epcDoc->getHdfProxySet()[0];

	// creating the ContinuousProperty
	RESQML2_0_1_NS::ContinuousProperty* continuousProperty = epcDoc->createContinuousProperty(
		frame, this->uuid, this->title,
		1,
		gsoap_resqml2_0_1::resqml2__IndexableElements__nodes,
		gsoap_resqml2_0_1::resqml2__ResqmlUom__m,
		gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind__length);
	REQUIRE(continuousProperty != nullptr);
	double values[5] = { 0.1, 1.2, 2.3, 3.4, 4.5 };
	continuousProperty->pushBackDoubleHdf5Array1dOfValues(values, 5, hdfProxy);

	// cleaning
	delete frameTest;
}

void ContinuousPropertyOnWellFrameTest::readEpcDocHandler() {
	// reading dependencies
	WellboreFrameRepresentationTest * frameTest = new WellboreFrameRepresentationTest(this->epcDoc, false);

	// getting the ContinuousPropertySeries
	RESQML2_0_1_NS::ContinuousProperty* continuousProperty = this->epcDoc->getResqmlAbstractObjectByUuid<RESQML2_0_1_NS::ContinuousProperty>(uuid);

	// ************************************
	// reading the ContinuousProperty

	// getElementCountPerValue
	REQUIRE(continuousProperty->getElementCountPerValue() == 1);

	// getAttachmentKind
	REQUIRE(continuousProperty->getAttachmentKind() == gsoap_resqml2_0_1::resqml2__IndexableElements__nodes);

	// getUom
	REQUIRE(continuousProperty->getUom() == gsoap_resqml2_0_1::resqml2__ResqmlUom__m);

	// getEnergisticsPropertyKind
	REQUIRE(continuousProperty->isAssociatedToOneStandardEnergisticsPropertyKind());
	REQUIRE(continuousProperty->getEnergisticsPropertyKind() == gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind__length);

	// getValuesCountOfPatch
	REQUIRE(continuousProperty->getValuesCountOfPatch(0) == 5);

	double* values = new double[5];
	continuousProperty->getDoubleValuesOfPatch(0, values);
	REQUIRE(values[0] == 0.1);
	REQUIRE(values[1] == 1.2);
	REQUIRE(values[2] == 2.3);
	REQUIRE(values[3] == 3.4);
	REQUIRE(values[4] == 4.5);
	delete[] values;

	// cleaning
	delete frameTest;
}

