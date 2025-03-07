#include <config/config.hxx>
#include <config/container/list.hxx>
#include <config/comment/multi.hxx>
#include <config/comment/single.hxx>
#include <util/system.hxx>

#include <iostream>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <climits>
#include "test_handlers.h"

using namespace StormByte::Config;

int test_add_and_lookup() {
	int result = 0;
    Config config;

    // Add Integer and String items
    config.Add(Item("TestInt", 42));
    config.Add(Item("TestStr", "Hello, World!"));

	try {
		// Lookup and validate items
		const Item& lookup_int = config["TestInt"];
		ASSERT_EQUAL("test_add_and_lookup", 42, lookup_int.Value().Get<int>());

		const Item& lookup_str = config["TestStr"];
		ASSERT_EQUAL("test_add_and_lookup", "Hello, World!", lookup_str.Value().Get<std::string>());
	}
	catch(const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result = 1;
	}

    RETURN_TEST("test_add_and_lookup", result);
}

int test_write_and_read() {
	int result = 0;
    std::filesystem::path temp_file = StormByte::Util::System::TempFileName();
    std::string config_content = 
        "TestInt = 42\n"
        "TestStr = \"Hello, World!\"\n";

    Config config;

	try {
		// Read from string
		config << config_content;

		// Validate items
		const Item& int_item = config["TestInt"];
		ASSERT_EQUAL("test_write_and_read", 42, int_item.Value().Get<int>());

		const Item& str_item = config["TestStr"];
		ASSERT_EQUAL("test_write_and_read", "Hello, World!", str_item.Value().Get<std::string>());

		// Write to file
		std::fstream file;
		file.open(temp_file, std::ios::out);
		file << config;
		file.close();

		// Read again from file to ensure correctness
		file.open(temp_file, std::ios::in);
		Config config2;
		file >> config2;
		file.close();

		const Item& int_item2 = config["TestInt"];
		ASSERT_EQUAL("test_write_and_read", 42, int_item2.Value().Get<int>());

		const Item& str_item2 = config["TestStr"];
		ASSERT_EQUAL("test_write_and_read", "Hello, World!", str_item2.Value().Get<std::string>());
	}
	catch(const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result = 1;
	}

    std::remove(temp_file.string().c_str());
    RETURN_TEST("test_write_and_read", result);
}

int test_nested_groups() {
	int result = 0;
    Config config;

	try {
		// Create nested groups
		Item& group1 = config.Add(Item("Group1", Container::Group()));
		Item& group2 = group1.Value().Get<Container::Base>().Add(Item("Group2", Container::Group()));

		// Add items to sub-group
		group2.Value().Get<Container::Base>().Add(Item("SubTestInt", 99));
		group2.Value().Get<Container::Base>().Add(Item("SubTestStr", "Sub Hello"));

		// Lookup and validate items
		const Item& lookup_int = config["Group1/Group2/SubTestInt"];
		ASSERT_EQUAL("test_nested_groups", 99, lookup_int.Value().Get<int>());

		const Item& lookup_str = config["Group1/Group2/SubTestStr"];
		ASSERT_EQUAL("test_nested_groups", "Sub Hello", lookup_str.Value().Get<std::string>());
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}

    RETURN_TEST("test_nested_groups", result);
}

int test_add_remove_group() {
	int result = 0;
    Config config;

	try {
		// Add group and items
		Container::Group group;
		group.Add(Item("GroupInt", 55));
		Item& group_item = config.Add(Item("TestGroup", group));

		// Remove the item from the group
		group_item.Value().Get<Container::Base>().Remove("GroupInt");

		// Validate removal
		config["TestGroup/GroupInt"];
		// Item not removed properly
		result = 1;
	}
	catch(const StormByte::Config::Exception&) {
		// Expected exception
		result = 0;
	}

    RETURN_TEST("test_add_remove_group", result);
}

int test_write_nested_groups() {
	int result = 0;
    std::filesystem::path temp_file = StormByte::Util::System::TempFileName();
    std::string config_content = 
        "Group1 = {\n"
        "    Group2 = {\n"
        "        SubTestInt = 99\n"
        "        SubTestStr = \"Sub Hello\"\n"
        "    }\n"
        "}\n";

    Config config;

	try {
		// Read from string
		config_content >> config;

		// Validate items
		const Item& lookup_int = config["Group1/Group2/SubTestInt"];
		ASSERT_EQUAL("test_write_nested_groups", 99, lookup_int.Value().Get<int>());

		const Item& lookup_str = config["Group1/Group2/SubTestStr"];
		ASSERT_EQUAL("test_write_nested_groups", "Sub Hello", lookup_str.Value().Get<std::string>());

		// Write to file
		std::fstream file;
		file.open(temp_file, std::ios::out);
		file << config;
		file.close();

		// Read again from file to ensure correctness
		file.open(temp_file, std::ios::in);
		Config config2;
		config2 << file;
		file.close();

		const Item& lookup_int2 = config2["Group1/Group2/SubTestInt"];
		ASSERT_EQUAL("test_write_nested_groups", 99, lookup_int2.Value().Get<int>());

		const Item& lookup_str2 = config2["Group1/Group2/SubTestStr"];
		ASSERT_EQUAL("test_write_nested_groups", "Sub Hello", lookup_str2.Value().Get<std::string>());
	}
	catch(const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result = 1;
	}

    std::remove(temp_file.string().c_str());
    RETURN_TEST("test_write_nested_groups", result);
}

int test_complex_config_creation() {
	int result = 0;
    std::filesystem::path temp_file = StormByte::Util::System::TempFileName();
    Config config;

	try {
		// Create a complex configuration
		Item& group1 = config.Add(Item("Group1", Container::Group()));
		Item& group2 = group1.Value().Get<Container::Base>().Add(Item("Group2", Container::Group()));

		group2.Value().Get<Container::Base>().Add(Item("IntItem1", 123));
		group2.Value().Get<Container::Base>().Add(Item("StrItem1", "Nested String"));

		Item& group3 = config.Add(Item("Group3", Container::Group()));
		group3.Value().Get<Container::Base>().Add(Item("IntItem2", 456));

		// Write to a temporary file
		std::fstream file;
		file.open(temp_file, std::ios::out);
		file << config;
		file.close();

		// Validate the written content
		std::ifstream temp_file_stream(temp_file);
		std::stringstream buffer;
		buffer << temp_file_stream.rdbuf();

		std::string expected_content = 
			"Group1 = {\n"
			"\tGroup2 = {\n"
			"\t\tIntItem1 = 123\n"
			"\t\tStrItem1 = \"Nested String\"\n"
			"\t}\n"
			"}\n"
			"Group3 = {\n"
			"\tIntItem2 = 456\n"
			"}\n";

		ASSERT_EQUAL("test_complex_config_creation", expected_content, buffer.str());
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}

    std::remove(temp_file.string().c_str());
    RETURN_TEST("test_write_nested_groups", result);
}

int bad_config1() {
	int result = 0;
	Config cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "bad_config1.conf", std::ios::in);
		cfg << file;
		file.close();
		result = 1;
		std::cout << "No exception here" << std::endl;
	}
	catch(const StormByte::Config::Exception&) {
		//Expected
		result = 0;
	}
	
	RETURN_TEST("bad_config1", result);
}

int bad_config2() {
	int result = 0;
	Config cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "bad_config2.conf", std::ios::in);
		cfg << file;
		file.close();
		result = 1;
	}
	catch(const StormByte::Config::Exception&) {
		//Expected
	}
	
	RETURN_TEST("bad_config2", result);
}

int bad_config3() {
	int result = 0;
	Config cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "bad_config3.conf", std::ios::in);
		cfg << file;
		file.close();
		result = 1;
	}
	catch(const StormByte::Config::Exception&) {
		//Expected
		result = 0;
	}
	
	RETURN_TEST("bad_config3", result);
}

int good_double_conf1() {
	int result = 0;
	Config cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "good_double_conf1.conf", std::ios::in);
		cfg << file;
		file.close();
		Item& lookup_double = cfg["test_double"];
		ASSERT_EQUAL("good_double_conf1", 666.666, lookup_double.Value().Get<double>());
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}
	
	RETURN_TEST("good_double_conf1", result);
}

int good_double_conf2() {
	int result = 0;
	Config cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "good_double_conf2.conf", std::ios::in);
		cfg << file;
		file.close();
		Item& lookup_test_double = cfg["test_double"];
		ASSERT_EQUAL("good_double_conf2", 19.89, lookup_test_double.Value().Get<double>());
		Item& lookup_test_exp = cfg["test_exp"];
		ASSERT_EQUAL("good_double_conf2", 1.87e-6, lookup_test_exp.Value().Get<double>());
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}
	
	RETURN_TEST("good_double_conf2", result);
}

int commented_config() {
	int result = 0;
	const std::filesystem::path temp_file = StormByte::Util::System::TempFileName();
	Config config;
	const std::string config_str = "# The following is a test integer\n"
		"test_integer = 666\n"
		"\n"
		"# Now a group\n"
		"test_group = { # We can have a comment here!\n"
		"\t# And also here\n"
		"\ttest_string = \"# But this is not a comment\"\n"
		"}\n"
		"# Ending comment";
	const std::string expected_str = "# The following is a test integer\n"
		"test_integer = 666\n"
		"# Now a group\n"
		"test_group = {\n"
		"\t# We can have a comment here!\n"
		"\t# And also here\n"
		"\ttest_string = \"# But this is not a comment\"\n"
		"}\n"
		"# Ending comment\n";

	try {
		config << config_str;
		std::fstream file;
		file.open(temp_file, std::ios::out);
		config >> file;
		file.close();

		const Item& test_string = config["test_group/test_string"];
		ASSERT_EQUAL("commented_config", "# But this is not a comment", test_string.Value().Get<std::string>());

		// Validate the written content
		std::ifstream temp_file_stream(temp_file);
		std::stringstream buffer;
		buffer << temp_file_stream.rdbuf();

		ASSERT_EQUAL("commented_config", expected_str, buffer.str());
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}

    std::remove(temp_file.string().c_str());

	RETURN_TEST("commented_config", result);
}

int good_string_conf() {
	int result = 0;
	Config cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "good_string_conf.conf", std::ios::in);
		cfg << file;
		file.close();
		const Item& lookup_string = cfg["test_string"];
		ASSERT_EQUAL("good_string_conf", "This is a test string", lookup_string.Value().Get<std::string>());

		const Item& lookup_quoted = cfg["test_quoted"];
		ASSERT_EQUAL("good_string_conf", "This \"quote\" allows more things", lookup_quoted.Value().Get<std::string>());

		const Item& lookup_unfinished = cfg["test_unfinished"];
		ASSERT_EQUAL("good_string_conf", "When you see a \" you might have the start of a string", lookup_unfinished.Value().Get<std::string>());
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}
	
	RETURN_TEST("good_string_conf", result);
}

int test_empty_string() {
	int result = 0;
    Config config;

    config.Add(Item("EmptyString", ""));

	try {
    	const Item& lookup_str = config["EmptyString"];
    	ASSERT_EQUAL("test_empty_string", "", lookup_str.Value().Get<std::string>());
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}

    RETURN_TEST("test_empty_string", result);
}

int test_integer_boundaries() {
	int result = 0;
    Config config;

    config.Add(Item("MaxInt", INT_MAX));

    config.Add(Item("MinInt", INT_MIN));

	try {
		const Item& lookup_max_int = config["MaxInt"];
		ASSERT_EQUAL("test_integer_boundaries", INT_MAX, lookup_max_int.Value().Get<int>());

		const Item& lookup_min_int = config["MinInt"];
		ASSERT_EQUAL("test_integer_boundaries", INT_MIN, lookup_min_int.Value().Get<int>());
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}

    RETURN_TEST("test_integer_boundaries", result);
}

int test_special_characters_in_string() {
	int result = 0;
    Config config;

    config.Add(Item("SpecialChars", "Line1\nLine2\tTabbed"));

	try {
    	const Item& lookup_str = config["SpecialChars"];
    	ASSERT_EQUAL("test_special_characters_in_string", "Line1\nLine2\tTabbed", lookup_str.Value().Get<std::string>());
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}

    RETURN_TEST("test_special_characters_in_string", result);
}

int test_deeply_nested_groups() {
	int result = 0;
    Config config;

	try {
		Item& group1 = config.Add(Item("Group1", Container::Group()));
		Item& group2 = group1.Value().Get<Container::Base>().Add(Item("Group2", Container::Group()));
		Item& group3 = group2.Value().Get<Container::Base>().Add(Item("Group3", Container::Group()));
		Item& group4 = group3.Value().Get<Container::Base>().Add(Item("Group4", Container::Group()));

		group4.Value().Get<Container::Base>().Add(Item("DeepInt", 1234));

		const Item& lookup_int = config["Group1/Group2/Group3/Group4/DeepInt"];
		ASSERT_EQUAL("test_deeply_nested_groups", 1234, lookup_int.Value().Get<int>());
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}

    RETURN_TEST("test_deeply_nested_groups", result);
}

int test_invalid_syntax() {
	int result = 0;
    Config config;
    std::string invalid_config = "Invalid = { Unclosed }";

    try {
        config << invalid_config;
        result = 1;
    } catch (const StormByte::Config::ParseError&) {
        // Expected exception
    }

    RETURN_TEST("test_invalid_syntax", result);
}

int test_special_characters_string() {
	int result = 0;
    Config cfg;
    try {
        std::fstream file;
		file.open(CurrentFileDirectory / "files" / "special_characters_conf.conf", std::ios::in);
		cfg << file;
		file.close();
        const Item& lookup_special = cfg["special_string"];
        ASSERT_EQUAL("test_special_characters_string", "This is a test string with special characters: \n, \t, \\", lookup_special.Value().Get<std::string>());
    } catch (const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
        result = 1;
    }

	RETURN_TEST("test_special_characters_string", result);
}

int test_long_string() {
	int result = 0;
    Config cfg;
    try {
        std::fstream file;
		file.open(CurrentFileDirectory / "files" / "long_string_conf.conf", std::ios::in);
		cfg << file;
		file.close();
        const Item& lookup_long = cfg["long_string"];
        ASSERT_EQUAL("test_long_string", std::string(1000, 'a'), lookup_long.Value().Get<std::string>());
    } catch (const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
        result = 1;
    }

	RETURN_TEST("test_long_string", result);
}

int test_unmatched_braces() {
	int result = 0;
    Config cfg;
    try {
        std::fstream file;
		file.open(CurrentFileDirectory / "files" / "unmatched_braces.conf", std::ios::in);
		cfg << file;
		file.close();
        result = 1;
    } catch (const StormByte::Config::Exception&) {
        // Expected
    }
    
	RETURN_TEST("test_unmatched_braces", result);
}

int good_boolean_config1() {
	int result = 0;
	Config cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "good_boolean_conf1.conf", std::ios::in);
		cfg << file;
		file.close();
		const Item& lookup_enable_feature = cfg["settings/enable_feature"];
        ASSERT_EQUAL("good_boolean_config1", true, lookup_enable_feature.Value().Get<bool>());

		const Item& lookup_enable_extra = cfg["settings/enable_extra"];
        ASSERT_EQUAL("good_boolean_config1", false, lookup_enable_extra.Value().Get<bool>());
	}
	catch (const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result = 1;
	}
	
	RETURN_TEST("good_boolean_config1", result);
}

int bad_boolean_config1() {
	int result = 0;
	Config cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "bad_boolean_conf1.conf", std::ios::in);
		cfg << file;
		file.close();
		result = 1;
	}
	catch (const StormByte::Config::Exception&) {
		// Expected exception
	}
	
	RETURN_TEST("bad_boolean_config1", result);
}

int copy_configuration() {
	Config cfg1, cfg2;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "good_boolean_conf1.conf", std::ios::in);
		cfg1 << file;
		file.close();
		cfg2 = cfg1;
	}
	catch(const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		RETURN_TEST("copy_configuration", 1);
	}

	try {
		// Both should be found
		const Item& lookup_enable_feature_1 = cfg1["settings/enable_feature"];
		const Item& lookup_enable_feature_2 = cfg2["settings/enable_feature"];
        ASSERT_EQUAL("good_boolean_config1", lookup_enable_feature_1.Value().Get<bool>(), lookup_enable_feature_2.Value().Get<bool>());
	}
	catch(const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		RETURN_TEST("copy_configuration", 1);
	}

	RETURN_TEST("copy_configuration", 0);
}

int move_configuration() {
	int result = 0;
	Config cfg1, cfg2;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "good_boolean_conf1.conf", std::ios::in);
		cfg1 << file;
		file.close();
		cfg2 = std::move(cfg1);
	}
	catch(const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		RETURN_TEST("move_configuration", 1);
	}

	// First should fail, second should be found
	try {
		cfg1["settings/enable_feature"];
		RETURN_TEST("move_configuration", 1);
	}
	catch(const StormByte::Config::Exception&) {
		// Expected
	}
	try {
		const Item& lookup_enable_feature = cfg2["settings/enable_feature"];
        ASSERT_EQUAL("move_configuration", true, lookup_enable_feature.Value().Get<bool>());
	}
	catch(const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		RETURN_TEST("move_configuration", 1);
	}

	RETURN_TEST("move_configuration", result);
}

int duplicated_insertion() {
	int result = 0;
	Config cfg;
	try {
		cfg.Add(Item("testInt", 66));
		cfg.Add(Item("testInt", 66));
		result = 1;
	}
	catch(const StormByte::Config::Exception&) {
		// Expected
	}

	return result;
}

int on_name_clash_keep_existing() {
	int result = 0;
	Config cfg;
	cfg.SetOnExistingAction(Container::Base::OnExistingAction::Keep);
	cfg.Add(Item("testItem", true));
	try {
		cfg.Add(Item("testItem", 666));
		// Should not throw because action was set to keep existing
		const Item& item = cfg["testItem"];
		ASSERT_EQUAL("on_name_clash_keep_existing", true, item.Value().Get<bool>());
	}
	catch(const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result = 1;
	}

	RETURN_TEST("on_name_clash_keep_existing", result);
}

int on_name_clash_replace() {
	int result = 0;
	Config cfg;
	cfg.SetOnExistingAction(Container::Base::OnExistingAction::Overwrite);
	cfg.Add(Item("testItem", true));
	try {
		cfg.Add(Item("testItem", 66));
		const Item& item = cfg["testItem"];
		ASSERT_EQUAL("on_name_clash_ignore", 66, item.Value().Get<int>());
	}
	catch(const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result = 1;
	}

	RETURN_TEST("on_name_clash_replace", result);
}

int config_to_config_output() {
	int result = 0;
	Config cfg1, cfg2;
	int value = 0;
	cfg1.Add(Item("testInt", value));
	cfg2.Add(Item("testString", "Hello!"));
	try {
		cfg1 << cfg2;
		const Item& testInt = cfg1["testInt"];
		ASSERT_EQUAL("config_to_config_output", 0, testInt.Value().Get<int>());
		const Item& testString = cfg1["testString"];
		ASSERT_EQUAL("config_to_config_output", "Hello!", testString.Value().Get<std::string>());
	}
	catch(const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result = 1;
	}

	RETURN_TEST("config_to_config_output", result);
}

int config_value_reference_change() {
	int result = 0;
	Config cfg;
	cfg.Add(Item("testInt", 66));

	try {
		cfg["testInt"].Value().Get<int>() = 99;
		const Item& testInt = cfg["testInt"];
		ASSERT_EQUAL("config_value_reference_change", 99, testInt.Value().Get<int>());
	}
	catch(const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result = 1;
	}
	return result;
}

int config_remove_full_path() {
	int result = 0;
	Config cfg;
	Item& group = cfg.Add(Item("testGroup", Container::Group()));
	group.Value().Get<Container::Base>().Add(Item("testInt", 99));
	group.Value().Get<Container::Base>().Add(Item("testString", "Group String"));

	try {
		cfg.Remove("testGroup/testInt");
		auto testString = cfg["testGroup/testString"];
		ASSERT_EQUAL("config_remove_full_path", "Group String", testString.Value().Get<std::string>());
	}
	catch(const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result = 1;
	}

	// Now testGroup/testInt should not exist
	try {
		cfg["testGroup/testInt"];
		result = 1;
	}
	catch(const StormByte::Config::Exception&) {
		// Expected
	}

	return result;
}

// Empty names are only allowed for comments (internally)
int config_test_add_empty_name() {
	int result = 0;
	Config cfg;
	try {
		cfg.Add(Item("", 66));
		std::cerr << "config_test_add_empty_name: Empty name was added when it should not be alloed" << std::endl;
		result = 1;
	}
	catch(const StormByte::Config::Exception&) {
		// Expected
	}
	return result;
}

// List test
int config_list_test() {
	Config cfg;
	cfg.Add(Item("testList", Container::List()));
	Item& list = cfg["testList"];
	list.Value().Get<Container::Base>().Add(Item(Comment::SingleLine("List comment")));
	list.Value().Get<Container::Base>().Add(Item(66));
	list.Value().Get<Container::Base>().Add(Item("Test string"));
	cfg.Add(Item("testGroup", Container::Group()));
	Item& group = cfg["testGroup"];
	group.Value().Get<Container::Base>().Add(Item("testInt", 99));
	group.Value().Get<Container::Base>().Add(Item("testString2", "Group String"));
	group.Value().Get<Container::Base>().Add(Item("testList2", Container::List()));
	Item& list2 = group.Value().Get<Container::Base>()["testList2"];
	list2.Value().Get<Container::Base>().Add(Item(Comment::SingleLine("List comment 2")));
	list2.Value().Get<Container::Base>().Add(Item(11));

	const std::string expected = "testList = [\n"
    "\t#List comment\n"
    "\t66\n"
    "\t\"Test string\"\n"
	"]\n"
	"testGroup = {\n"
    "\ttestInt = 99\n"
    "\ttestString2 = \"Group String\"\n"
    "\ttestList2 = [\n"
    "\t\t#List comment 2\n"
    "\t\t11\n"
    "\t]\n"
	"}\n";

	Config cfg2;
	try {
		cfg2 << cfg;
		ASSERT_EQUAL("config_list_test", expected, (std::string)cfg2);
	}
	catch(const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		RETURN_TEST("config_list_test", 1);
	}

	RETURN_TEST("config_list_test", 0);
}

// Test list access
int config_list_access_by_index() {
	Config cfg1;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "good_list_conf1.conf", std::ios::in);
		cfg1 << file;
		file.close();
		const auto& lookup_list = cfg1["testList"].Value().Get<Container::Base>();
		ASSERT_EQUAL("config_list_access_by_index", 66, lookup_list[1].Value().Get<int>());
		const auto& lookup_list2 = cfg1["testGroup/testList2"].Value().Get<Container::Base>();
		ASSERT_EQUAL("config_list_access_by_index", 11, lookup_list2[1].Value().Get<int>());
	}
	catch(const StormByte::Config::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		RETURN_TEST("config_list_access_by_index", 1);
	}
	RETURN_TEST("config_list_access_by_index", 0);
}

int complex_conf1() {
	int result = 0;
	Config cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "complex_conf1.conf", std::ios::in);
		cfg << file;
		file.close();
		const Item& lookup_testInt = cfg["testGroup/testList2"];
		const Item& group_inside_list = lookup_testInt.Value().Get<Container::Base>()[3];
		const Item& lookup_testInt_inside = group_inside_list.Value().Get<Container::Base>()["testInt"];
		ASSERT_EQUAL("complex_conf1", 1, lookup_testInt_inside.Value().Get<int>());
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}
	
	RETURN_TEST("complex_conf1", result);
}

int copy_and_delete() {
	int result = 0;
	std::unique_ptr<Config> cfg(std::make_unique<Config>());
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "complex_conf1.conf", std::ios::in);
		*cfg << file;
		file.close();
		const Config cfg2(*cfg);
		cfg.reset();
		const Item& lookup_testInt = cfg2["testGroup/testList2"];
		const Item& group_inside_list = lookup_testInt.Value().Get<Container::Base>()[3];
		const Item& lookup_testInt_inside = group_inside_list.Value().Get<Container::Base>()["testInt"];
		ASSERT_EQUAL("copy_and_delete", 1, lookup_testInt_inside.Value().Get<int>());
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}

	RETURN_TEST("copy_and_delete", result);
}

int complex_path_access() {
	int result = 0;
	Config cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "complex_conf1.conf", std::ios::in);
		cfg << file;
		file.close();
		const Item& lookup_deep_into_list = cfg["testGroup/testList2/3/testList/2"];
		ASSERT_EQUAL("complex_path_access", 3, lookup_deep_into_list.Value().Get<int>());
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}
	
	RETURN_TEST("complex_path_access", result);
}

int good_comment_multi_conf1() {
	int result = 0;
	Config cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "good_comment_multi_conf1.conf", std::ios::in);
		cfg << file;
		file.close();
		const std::string expected = 
		"# This is a comment\n"
		"testInteger = 6\n"
		"/*\n"
 		" * This is a block comment\n"
		" */\n"
		"testString = \"test\"\n"
		"/* This is a comment */\n"
		"testGroup = {\n"
		"\t/**\n"
		"\t * This is a documentation comment\n"
		"\t */\n"
		"\t/* testInt = 6 */\n"
		"\ttestString = \"test2\"\n"
		"}\n";
		const std::string actual = (std::string)cfg;
		ASSERT_EQUAL("good_comment_conf1", expected, actual);
	}
	catch(const StormByte::Config::Exception&) {
		result = 1;
	}
	
	RETURN_TEST("good_double_conf2", result);
}

int test_config_hooks() {
	int result = 0;
	Config cfg1;
	cfg1.AddHookAfterRead([](Config& cfg) {
		cfg.Clear();
	});
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "complex_conf1.conf", std::ios::in);
		cfg1 << file;
		file.close();
		ASSERT_EQUAL("test_config_hooks", 0, cfg1.Size());
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}
	return result;
}

int size_and_count() {
	int result = 0;
	Config cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "complex_conf1.conf", std::ios::in);
		cfg << file;
		file.close();
		const size_t size = cfg.Size();
		const size_t count = cfg.Count();
		ASSERT_EQUAL("size_and_count", 4, size);
		ASSERT_EQUAL("size_and_count", 24, count);
	}
	catch(const StormByte::Config::Exception& e) {
		std::cerr << e.what() << std::endl;
		result = 1;
	}
	
	RETURN_TEST("complex_conf1", result);
}

int main() {
    int result = 0;
    try {
		result += test_add_and_lookup();
		result += test_write_and_read();
		result += test_nested_groups();
		result += test_add_remove_group();
		result += test_write_nested_groups();
		result += test_complex_config_creation();
		result += bad_config1();
		result += bad_config2();
		result += bad_config3();
		result += good_double_conf1();
		result += good_double_conf2();
		result += commented_config();
		result += good_string_conf();
		result += test_empty_string();
		result += test_integer_boundaries();
		result += test_special_characters_in_string();
		result += test_deeply_nested_groups();
		result += test_invalid_syntax();
		result += test_special_characters_string();
		result += test_long_string();
		result += test_unmatched_braces();
		result += good_boolean_config1();
		result += bad_boolean_config1();
		result += copy_configuration();
		result += move_configuration();
		result += duplicated_insertion();
		result += on_name_clash_keep_existing();
		result += on_name_clash_replace();
		result += config_to_config_output();
		result += config_value_reference_change();
		result += config_remove_full_path();
		result += config_test_add_empty_name();
		result += config_list_test();
		result += config_list_access_by_index();
		result += complex_conf1();
		result += copy_and_delete();
		result += complex_path_access();
		result += good_comment_multi_conf1();
		result += test_config_hooks();
		result += size_and_count();
    } catch (const StormByte::Config::Exception& ex) {
        std::cerr << ex.what() << std::endl;
        result++;
    }
    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }
    return result;
}
