#include <catch2/catch.hpp>

#include <pf/new.hpp>

#include "./compare_fs.hpp"

namespace fs = pf::fs;

namespace {

fs::path create_project(const pf::new_project_params& params) {
    std::error_code ec;
    INFO("Project name: " + params.name);
    INFO("Project namespace: " + params.root_namespace);
    INFO(params.directory)
    fs::remove_all(params.directory, ec);
    if (ec && ec != std::errc::no_such_file_or_directory) {
        REQUIRE_FALSE(ec);
    }
    pf::create_project(params, ec);
    REQUIRE_FALSE(ec);
    return params.directory;
}

pf::new_project_params make_project_params(const std::string& name, const std::string& ns) {
    return pf::new_project_params{name,
                                  ns,
                                  name,
                                  fs::path{PF_TEST_BINDIR} / "_gen_projects" / name};
}

fs::path expected_for(const std::string& name) {
    return fs::path{PF_TEST_SRCDIR} / "expected" / name;
}

void generate_and_compare(const pf::new_project_params& params, const std::string& name) {
    auto dir  = create_project(params);
    auto diff = pf::test::compare_fs_tree(dir, expected_for(name));
    CHECK_FALSE(diff);
}

}  // namespace

TEST_CASE("simple_project") {
    auto params = make_project_params("simple", "simple");
    generate_and_compare(params, "simple");
}
