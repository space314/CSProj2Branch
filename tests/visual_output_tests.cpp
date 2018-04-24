#include "../shape.hpp"
#include <iostream>
#include <memory>
#include <string>

int main()
{
    // Tests for circle
	auto test_circle_radius_0 = std::make_unique<Circle>(0);
    test_circle_radius_0->generate_postscript_file("cache_circle_radius_0");
    auto test_circle_radius_50 = std::make_unique<Circle>(50);
    test_circle_radius_50->generate_postscript_file("cache_circle_radius_50");
    auto test_circle_radius_100 = std::make_unique<Circle>(100);
    test_circle_radius_100->generate_postscript_file("cache_circle_radius_100");

    // Testing for Diamond
    auto diamond_side_length_0 = std::make_unique<Diamond>(0);
    diamond_side_length_0->generate_postscript_file("cache_diamond_side_length_0");
    auto diamond_side_length_50 = std::make_unique<Diamond>(50);
    diamond_side_length_50->generate_postscript_file("cache_diamond_side_length_50");
    auto diamond_side_length_100 = std::make_unique<Diamond>(100);
    diamond_side_length_100->generate_postscript_file("cache_diamond_side_length_100");

	// // auto test2 = std::make_unique<Layered>(std::move(test1), 4,4);
    // auto test3 = std::make_unique<Circle>(3.);
    // auto test4 = std::make_unique<Polygon>(7,50);
    // auto test5 = std::make_unique<Spacer>(100,100);
    // auto test2 = std::make_unique<Horizontal>(std::initializer_list<std::shared_ptr<Shape>>{std::move(test1), std::move(test3), std::move(test5), std::move(test4)});
    // // std::cout << test1.to_postscript() << std::endl;
}