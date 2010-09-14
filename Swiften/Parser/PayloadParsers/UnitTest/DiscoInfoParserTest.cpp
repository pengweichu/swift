/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "Swiften/Parser/PayloadParsers/DiscoInfoParser.h"
#include "Swiften/Parser/PayloadParsers/UnitTest/PayloadsParserTester.h"

using namespace Swift;

class DiscoInfoParserTest : public CppUnit::TestFixture
{
		CPPUNIT_TEST_SUITE(DiscoInfoParserTest);
		CPPUNIT_TEST(testParse);
		CPPUNIT_TEST(testParse_Form);
		CPPUNIT_TEST_SUITE_END();

	public:
		DiscoInfoParserTest() {}

		void testParse() {
			PayloadsParserTester parser;

			CPPUNIT_ASSERT(parser.parse(
				"<query xmlns=\"http://jabber.org/protocol/disco#info\">"
					"<identity name=\"Swift\" category=\"client\" type=\"pc\" xml:lang=\"en\"/>" 
					"<identity name=\"Vlug\" category=\"client\" type=\"pc\" xml:lang=\"nl\"/>" 
					"<feature var=\"foo-feature\"/>"
					"<feature var=\"bar-feature\"/>"
					"<feature var=\"baz-feature\"/>"
				"</query>"));

			DiscoInfo* payload = dynamic_cast<DiscoInfo*>(parser.getPayload().get());
			CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(payload->getIdentities().size()));
			CPPUNIT_ASSERT_EQUAL(String("Swift"), payload->getIdentities()[0].getName());
			CPPUNIT_ASSERT_EQUAL(String("pc"), payload->getIdentities()[0].getType());
			CPPUNIT_ASSERT_EQUAL(String("client"), payload->getIdentities()[0].getCategory());
			CPPUNIT_ASSERT_EQUAL(String("en"), payload->getIdentities()[0].getLanguage());
			CPPUNIT_ASSERT_EQUAL(String("Vlug"), payload->getIdentities()[1].getName());
			CPPUNIT_ASSERT_EQUAL(String("pc"), payload->getIdentities()[1].getType());
			CPPUNIT_ASSERT_EQUAL(String("client"), payload->getIdentities()[1].getCategory());
			CPPUNIT_ASSERT_EQUAL(String("nl"), payload->getIdentities()[1].getLanguage());
			CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(payload->getFeatures().size()));
			CPPUNIT_ASSERT_EQUAL(String("foo-feature"), payload->getFeatures()[0]);
			CPPUNIT_ASSERT_EQUAL(String("bar-feature"), payload->getFeatures()[1]);
			CPPUNIT_ASSERT_EQUAL(String("baz-feature"), payload->getFeatures()[2]);
		}

		void testParse_Form() {
			PayloadsParserTester parser;

			CPPUNIT_ASSERT(parser.parse(
				"<query xmlns=\"http://jabber.org/protocol/disco#info\">"
					"<feature var=\"foo-feature\"/>"
					"<x type=\"submit\" xmlns=\"jabber:x:data\">"
						"<title>Bot Configuration</title>"
						"<instructions>Hello!</instructions>"
					"</x>"
					"<feature var=\"bar-feature\"/>"
				"</query>"));

			DiscoInfo* payload = dynamic_cast<DiscoInfo*>(parser.getPayload().get());
			CPPUNIT_ASSERT_EQUAL(1, static_cast<int>(payload->getExtensions().size()));
			CPPUNIT_ASSERT_EQUAL(String("Bot Configuration"), payload->getExtensions()[0]->getTitle());
			CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(payload->getFeatures().size()));
			CPPUNIT_ASSERT_EQUAL(String("foo-feature"), payload->getFeatures()[0]);
			CPPUNIT_ASSERT_EQUAL(String("bar-feature"), payload->getFeatures()[1]);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(DiscoInfoParserTest);
