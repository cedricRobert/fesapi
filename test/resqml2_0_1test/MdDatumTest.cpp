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
#include "resqml2_0_1test/MdDatumTest.h"

#include <stdexcept>

#include "catch.hpp"
#include "resqml2_0_1test/LocalDepth3dCrsTest.h"

#include "common/EpcDocument.h"
#include "resqml2_0_1/WellboreFeature.h"
#include "resqml2_0_1/LocalDepth3dCrs.h"

using namespace std;
using namespace resqml2_0_1test;
using namespace COMMON_NS;
using namespace RESQML2_NS;

const char* MdDatumTest::defaultUuid = "aa4da18f-5cc8-4bbb-841a-30e4031376fa";
const char* MdDatumTest::defaultTitle = "Md Datum";

MdDatumTest::MdDatumTest(const string & epcDocPath)
	: AbstractResqmlDataObjectTest(epcDocPath, defaultUuid, defaultTitle) {
}

MdDatumTest::MdDatumTest(EpcDocument* epcDoc, bool init)
	: AbstractResqmlDataObjectTest(epcDoc, defaultUuid, defaultTitle) {
	if (init)
			this->initEpcDoc();
		else
			this->readEpcDoc();
}

void MdDatumTest::initEpcDocHandler() {
	LocalDepth3dCrsTest * crsTest = new LocalDepth3dCrsTest(this->epcDoc, true);
	RESQML2_0_1_NS::LocalDepth3dCrs * crs = static_cast<RESQML2_0_1_NS::LocalDepth3dCrs *>(this->epcDoc->getResqmlAbstractObjectByUuid(LocalDepth3dCrsTest::defaultUuid));

	// cleaning
	delete crsTest;

	MdDatum* mdDatum = epcDoc->createMdDatum(uuid, title, crs, gsoap_resqml2_0_1::resqml2__MdReference__mean_x0020sea_x0020level, 275, 75, 0);
	REQUIRE(mdDatum != nullptr);
}

void MdDatumTest::readEpcDocHandler() {
}

