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
#include "resqml2_0_1test/InterpretationDomain.h"

#include "../catch.hpp"

#include "resqml2_0_1test/FaultInterpretationTest.h"
#include "resqml2_0_1test/FaultSinglePatchTriangulatedSetRepresentationTest.h"
#include "resqml2_0_1test/PolylineSetRepresentation.h"

#include "resqml2_0_1/FaultInterpretation.h"
#include <stdexcept>
#include "../config.h"

using namespace std;
using namespace resqml2_0_1test;
using namespace COMMON_NS;
using namespace RESQML2_0_1_NS;

InterpretationDomain::InterpretationDomain(const string & epcDocPath)
	: AbstractTest(epcDocPath)
{
}

InterpretationDomain::InterpretationDomain(EpcDocument* epcDoc, bool init)
	: AbstractTest(epcDoc)
{
	if (init)
		this->initEpcDoc();
	else
		this->readEpcDoc();
}

void InterpretationDomain::initEpcDoc() {

	FaultInterpretation* faultInterp = epcDoc->getResqmlAbstractObjectByUuid<FaultInterpretation>(uuidFaultInterpretation);
	if (faultInterp == nullptr) {
		FaultInterpretationTest* faultInterpTest = new FaultInterpretationTest(this->epcDoc, true);
		faultInterp = epcDoc->getResqmlAbstractObjectByUuid<FaultInterpretation>(uuidFaultInterpretation);
		delete faultInterpTest;
	}

	REQUIRE(faultInterp->getDomain() == gsoap_resqml2_0_1::resqml2__Domain__mixed);
	faultInterp->initDomain(gsoap_resqml2_0_1::resqml2__Domain__time);
	REQUIRE(faultInterp->getDomain() == gsoap_resqml2_0_1::resqml2__Domain__time);

	FaultSinglePatchTriangulatedSetRepresentationTest* repTest = new FaultSinglePatchTriangulatedSetRepresentationTest(epcDoc, true);
	REQUIRE(faultInterp->getDomain() == gsoap_resqml2_0_1::resqml2__Domain__depth);
	resqml2_0_1test::PolylineSetRepresentation* polylineRepTest = new resqml2_0_1test::PolylineSetRepresentation(epcDoc, true);
	REQUIRE(faultInterp->getDomain() == gsoap_resqml2_0_1::resqml2__Domain__mixed);

	delete repTest;
	delete polylineRepTest;
}

void InterpretationDomain::readEpcDoc() {

}

