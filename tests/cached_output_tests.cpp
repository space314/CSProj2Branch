#include "../shape.hpp"
#include <iostream>
#include <memory>
#include <string>

bool compare_file_content(std::string cached_file, std::string test_string)
{
    std::fstream open_cache(cached_file + ".ps", std::fstream::in);
    std::fstream open_test(test_string + ".ps", std::fstream::in);
    std::string cache_buffer;
    std::string test_buffer;

	if (!open_cache.is_open()) 
	{
    	std::cout << "file failed to open " << cached_file + ".ps" << std::endl;
  	} 
    if (!open_test.is_open()) 
	{
    	std::cout << "file failed to open " << test_string + ".ps" << std::endl;
  	} 
    std::getline(open_test, test_buffer);
    std::getline(open_cache, cache_buffer);
    while(!open_cache.eof() || !open_test.eof())
    {
        if(test_buffer != cache_buffer)
        {
            return false;
        }
        if(open_cache.eof() == ! open_test.eof())
        {
            return false;
        }
        std::getline(open_test, test_buffer);
        std::getline(open_cache, cache_buffer);
    }
    return true;
}

void test_case(std::string test_name, bool test_value)
{
    std::string test_value_text = "failed";
    if(test_value)
    {
        test_value_text = "passed";
    }
    std::cout << "TEST: " + test_name + "; " + test_value_text << std::endl;
}

int main()
{
    // Testing for Circle
    auto test_circle_radius_0 = std::make_unique<Circle>(0);
    test_circle_radius_0->generate_postscript_file("test_circle_radius_0");
    test_case("circle with radius 0", compare_file_content("cache_circle_radius_0", "test_circle_radius_0"));

    auto test_circle_radius_50 = std::make_unique<Circle>(50);
    test_circle_radius_50->generate_postscript_file("test_circle_radius_50");
    test_case("circle with radius 50", compare_file_content("cache_circle_radius_50", "test_circle_radius_50"));

    auto test_circle_radius_100 = std::make_unique<Circle>(100);
    test_circle_radius_100->generate_postscript_file("test_circle_radius_100");
    test_case("circle with radius 100", compare_file_content("cache_circle_radius_100", "test_circle_radius_100"));

    // Testing for Diamond
    auto diamond_side_length_0 = std::make_unique<Diamond>(0);
    diamond_side_length_0->generate_postscript_file("test_diamond_side_length_0");
    test_case("diamond with side length 0", compare_file_content("cache_diamond_side_length_0", "test_diamond_side_length_0"));

    auto diamond_side_length_50 = std::make_unique<Diamond>(50);
    diamond_side_length_50->generate_postscript_file("test_diamond_side_length_50");
    test_case("diamond with side length 50", compare_file_content("cache_diamond_side_length_50", "test_diamond_side_length_50"));

    auto diamond_side_length_100 = std::make_unique<Diamond>(100);
    diamond_side_length_100->generate_postscript_file("test_diamond_side_length_100");
    test_case("diamond with side length 100", compare_file_content("cache_diamond_side_length_100", "test_diamond_side_length_100"));
    
	// Testing for polygons
	auto test_pentagon_0 = std::make_unique<Polygon>(5, 0.);
	test_pentagon_0->generate_postscript_file("test_pentagon_0");
	test_case("Pentagon with side length 0", compare_file_content("cache_test_pentagon_0", "test_pentagon_0"));

	auto test_pentagon_50 = std::make_unique<Polygon>(5, 50.);
	test_pentagon_50->generate_postscript_file("test_pentagon_50");
	test_case("Pentagon with side length 50", compare_file_content("cache_test_pentagon_50", "test_pentagon_50"));

	auto test_pentagon_100 = std::make_unique<Polygon>(5, 100.);
	test_pentagon_100->generate_postscript_file("test_pentagon_100");
	test_case("Pentagon with side length 100", compare_file_content("cache_test_pentagon_100", "test_pentagon_100"));

	auto test_square_20 = std::make_unique<Square>(20.);
	test_square_20->generate_postscript_file("test_square_20");
	test_case("Square with side length 20", compare_file_content("cache_test_square_20", "test_square_20"));

	auto test_triangle_20 = std::make_unique<Triangle>(20.);
	test_triangle_20->generate_postscript_file("test_triangle_20");
	test_case("Triangle with side length 20", compare_file_content("cache_test_triangle_20", "test_triangle_20"));

	auto test_hexagon_0 = std::make_unique<Polygon>(5, 0.);
	test_hexagon_0->generate_postscript_file("test_hexagon_0");
	test_case("Hexagon with side length 0", compare_file_content("cache_test_hexagon_0", "test_hexagon_0"));

	auto test_hexagon_50 = std::make_unique<Polygon>(5, 50.);
	test_hexagon_50->generate_postscript_file("test_hexagon_50");
	test_case("Hexagon with side length 50", compare_file_content("cache_test_hexagon_50", "test_hexagon_50"));

	auto test_hexagon_100 = std::make_unique<Polygon>(5, 100.);
	test_hexagon_100->generate_postscript_file("test_hexagon_100");
	test_case("Hexagon with side length 100", compare_file_content("cache_test_hexagon_100", "test_hexagon_100"));

	// Testing for rotation
	auto rect = std::make_unique<Rectangle>(50., 20.);
	auto test_rot_q = std::make_unique<Rotated>(std::move(rect), Rotated::QUARTER);
	test_rot_q->generate_postscript_file("test_rot_q");
	test_case("Rotated Shape", compare_file_content("cache_test_rot_q", "test_rot_q"));

	// Testing for Horizontal
	auto square1 = std::make_unique<Square>(50.);
	auto square2 = std::make_unique<Square>(50.);
	auto square3 = std::make_unique<Square>(50.);
	auto test_horiz = std::make_unique<Horizontal>(std::initializer_list < std::shared_ptr<Shape>>{std::move(square1), std::move(square2), std::move(square3)});
	test_horiz->generate_postscript_file("test_horiz");
	test_case("Horizontal Shape", compare_file_content("cache_test_horiz", "test_horiz"));

	auto test_sierpinski = std::make_unique<STriangle>(200.,6);
	test_sierpinski->generate_postscript_file("test_sierpinski");
	test_case("sierpinski triangle with size 200 and recursion depth of 6", compare_file_content("cached_test_sierpinski","test_sierpinski"));

	auto test_U_curve = std::make_unique<U_Curve>(100.);
	test_U_curve->generate_postscript_file("test_U_curve");
	test_case("U_curve", compare_file_content("cache_test_U_curve", "test_U_curve"));


	auto test_LPB = std::make_unique<LPB>(100., 4);
	test_LPB->generate_postscript_file("test_LPB");
	test_case("Space filling curve", compare_file_content("cache_test_LPB", "test_LPB"));
	system("Pause");
}