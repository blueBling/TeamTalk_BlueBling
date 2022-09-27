//
//  test_jsonxx.cpp
//  test_jsonxx
//
//  Created by blueBling on 22-07-25.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "jsonxx.h"

#include <iostream>
#include <string>


using std::cout;
using std::endl;
using std::string;
using namespace jsonxx;

string teststr(
        "{"
        "  \"foo\" : 1,"
        "  \"bar\" : false,"
        "  \"person\" : {\"name\" : \"GWB\", \"age\" : 60,},"
        "  \"data\": [\"abcd\", 42],"
        "}"
);


int test_jsonxx(){
	
	// Parse string or stream
	Object o;
	assert(o.parse(teststr));

	// Validation. Checking for JSON types and values as well
	assert(1 == o.get<Number>("foo"));
	assert(o.has<Boolean>("bar"));
	assert(o.has<Object>("person"));
	assert(o.get<Object>("person").has<Number>("age"));
	assert(!o.get<Object>("person").has<Boolean>("old"));
	// assert(o.get<Object>("person").get<Boolean>("old", false));
	assert(o.has<Array>("data"));
	assert(o.get<Array>("data").get<Number>(1) == 42);
	assert(o.get<Array>("data").get<String>(0) == "abcd");
	// assert(o.get<Array>("data").get<String>(2, "hello") == "hello");
	assert(!o.has<Number>("data"));
	cout << o.json() << endl;                     // JSON output
	// cout << o.xml(JSONx) << endl;                 // JSON to XML conversion (JSONx subtype)
	// cout << o.xml(JXML) << endl;                  // JSON to XML conversion (JXML subtype)
	// cout << o.xml(JXMLex) << endl;                // JSON to XML conversion (JXMLex subtype)
	
	return 0;
}


int test_generate_json() {
	// Generate JSON document dynamically

	Array a;
	a << 123;
	a << "hello world";
	a << 3.1415;
	a << 99.95f;
	a << 'h';
	a << Object("key", "value");
	Object o;
	o << "key1" << "value";
	o << "key2" << 123;
	o << "key3" << a;
	cout << o.json() << endl;
	
	return 0;
}

int main(){
	test_jsonxx();
	test_generate_json();
	return 0;
}