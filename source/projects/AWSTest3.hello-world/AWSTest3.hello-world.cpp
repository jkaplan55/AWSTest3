/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.
 
#include "c74_min.h"
#include <aws/core/Aws.h>
#include <aws/core/utils/logging/LogLevel.h>
#include <aws/s3/S3Client.h>
#include <iostream>
using namespace c74::min;


class hello_world : public object<hello_world> {
public:
	MIN_DESCRIPTION {"Post to the Max Console."};
	MIN_TAGS {"utilities"};
	MIN_AUTHOR {"Cycling '74"};
	MIN_RELATED {"print, jit.print, dict.print"};

	inlet<>  input {this, "(bang) post greeting to the max console"};
	outlet<> output {this, "(anything) output the message which is posted to the max console"};

	hello_world() {
		c74::max::common_symbols_init();
	}


	void convertJSONtoDict() {
		using namespace c74::max;
		t_dictionary* d = NULL;
		dict          e {c74::min::symbol(true)};
		
		t_max_err   err;
		t_atom      result[1];
		t_object*   jsonreader = (t_object*)object_new(_sym_nobox, _sym_jsonreader);
		const char* jsontext   = "{\"name\":\"John\", \"age\":30, \"car\":null}";
		err                    = (t_max_err)object_method(jsonreader, _sym_parse, jsontext, result);
		if (!err) {
			t_object* ro = (t_object*)atom_getobj(result);
			if (ro) {
				if (object_classname_compare(ro, _sym_dictionary)) {
					d = (t_dictionary*)ro;
					dict mindict {d};
					e.copyunique(mindict);
					c74::min::symbol key {"name"};
					c74::min::atom   value = mindict[key].begin();
					cout << "key: " << key << ". Value: " << value << endl;
				}
				else
					object_free(ro);
			}
		}
		object_free(jsonreader);
		output.send("dictionary", e.name());
	}

	message<> test {this, "test", "Post the greeting.", MIN_FUNCTION {
    using namespace c74::max;
    t_dictionary* d = NULL;
	t_max_err   err;
	t_atom      result[1];
	t_object*   jsonreader = (t_object*)object_new(_sym_nobox, _sym_jsonreader);
	const char* jsontext   = "{\"name\":\"John\", \"age\":30, \"car\":null}";
	err                    = (t_max_err)object_method(jsonreader, _sym_parse, jsontext, result);
	if (!err) {
		t_object* ro = (t_object*)atom_getobj(result);
		if (ro) {
			if (object_classname_compare(ro, _sym_dictionary)) {
				d = (t_dictionary*)ro;
				t_symbol* dname = NULL;
				dictobj_register(d, &dname);
				dict             mindict {d};
				c74::min::symbol key {"name"};
				c74::min::atom   value = mindict[key].begin();
				cout << "key: " << key << ". Value: " << value << endl;
				output.send("dictionary", mindict.name());
				object_free(d);
				object_free(mindict);			
			}
			else
				object_free(ro);
		}
	}
	object_free(jsonreader);

	return {};
	}	
};
}
;
	

MIN_EXTERNAL(hello_world);
