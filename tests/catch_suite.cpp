//Tests for Shape Class

#define CATCH_CONFIG_FAST_COMPILE
# define M_PI  3.14159265358979323846

#include "catch.hpp"
#include "../shape.hpp"

#include <memory>
#include <utility>
#include <initializer_list>

TEST_CASE("Fail construction", "[construction]")
{

}

TEST_CASE("Circle set/get", "[set/get]")
{
	INFO("Constructor creation");
	auto circle = std::make_unique<Circle>(5.0);
	REQUIRE(circle->get_height() == 10.0);
	REQUIRE(circle->get_width() == 10.0);

	INFO("Set height");
	circle->set_height(50.0);
	REQUIRE(circle->get_height() == 50.0);
	REQUIRE(circle->get_width() == 10.0);

	INFO("Set width");
	circle->set_width(100.0);
	REQUIRE(circle->get_height() == 50.0);
	REQUIRE(circle->get_width() == 100.0);
}

TEST_CASE("Rectangle set/get", "[set/get]")
{
	INFO("Constructor creation");
	auto rect = std::make_unique<Rectangle>(10.0, 15.0);
	REQUIRE(rect->get_width() == 10.0);
	REQUIRE(rect->get_height() == 15.0);

	INFO("Set height");
	rect->set_height(50.0);
	REQUIRE(rect->get_width() == 10.0);
	REQUIRE(rect->get_height() == 50.0);

	INFO("Set width");
	rect->set_width(100.0);
	REQUIRE(rect->get_width() == 100.0);
	REQUIRE(rect->get_height() == 50.0);
}

TEST_CASE("Spacer set/get", "[set/get]")
{
	INFO("Constructor creation");
	auto space = std::make_unique<Spacer>(50.0, 50.0);
	REQUIRE(space->get_width() == 50.0);
	REQUIRE(space->get_height() == 50.0);

	INFO("Set height");
	space->set_height(200.0);
	REQUIRE(space->get_width() == 50.0);
	REQUIRE(space->get_height() == 200.0);

	INFO("Set width");
	space->set_width(100.0);
	REQUIRE(space->get_width() == 100.0);
	REQUIRE(space->get_height() == 200.0);
}

TEST_CASE("Square set/get", "[set/get]")
{
	INFO("Constructor creation");
	auto square = std::make_unique<Square>(50.0);
	square->set_width(50.0); //Remove
	square->set_height(50.0); //Remove
	REQUIRE(square->get_width() == 50.0);
	REQUIRE(square->get_height() == 50.0);

	INFO("Set height");
	square->set_height(200.0);
	square->set_width(200.0); //Remove
	REQUIRE(square->get_width() == 200.0);
	REQUIRE(square->get_height() == 200.0);

	INFO("Set width");
	square->set_width(100.0);
	square->set_height(100.0); //Remove
	REQUIRE(square->get_width() == 100.0);
	REQUIRE(square->get_height() == 100.0);
}

TEST_CASE("Triangle set/get", "[set/get]")
{
	const auto side_length = 50.0;
	const auto num_sides = 3;
	const auto cos_part = std::cos(M_PI / double(3));
	const auto sin_part = std::sin(M_PI / double(3));
	const auto height = side_length*(1 + cos_part) / (2 * sin_part);
	const auto width = (side_length * std::sin(M_PI* (num_sides - 1) / double(2 * num_sides))) / (sin_part);

	INFO("Constructor creation");
	auto tri = std::make_unique<Triangle>(50.0);
	REQUIRE(tri->get_width() == width);
	REQUIRE(tri->get_height() == height);

	INFO("Set height");
	tri->set_height(200.0);
	REQUIRE(tri->get_width() == width);
	REQUIRE(tri->get_height() == 200.0);

	INFO("Set width");
	tri->set_width(100.0);
	REQUIRE(tri->get_width() == 100.0);
	REQUIRE(tri->get_height() == 200.0);
}

TEST_CASE("Polygon set/test", "[set/get]")
{
	const auto side_length = 1.0;
	const auto num_sides = 5;
	const auto cos_part = std::cos(M_PI / double(5));
	const auto sin_part = std::sin(M_PI / double(5));
	const auto height = side_length*(1 + cos_part) / (2 * sin_part);
	const auto width = (side_length * std::sin(M_PI* (num_sides - 1) / double(2 * num_sides))) / (sin_part);
	auto pentagon = std::make_unique<Polygon>(5, 1.);
	INFO("Constructor");
	REQUIRE(pentagon->get_height() == height);
	REQUIRE(pentagon->get_width() == width);


	INFO("set height");
	pentagon->set_height(2.);
	REQUIRE(pentagon->get_height() == 2.);
	INFO("set width");
	pentagon->set_width(20.);
	REQUIRE(pentagon->get_width() == 20.);
}

TEST_CASE("Rotated set/get", "[set/get]")
{
	auto rect = std::make_unique<Rectangle>(100.0, 200.0);

	INFO("Constructor creation");
	auto rot0 = std::make_unique<Rotated>(std::move(rect), Rotated::QUARTER);
	REQUIRE(rot0->get_width() == 200.0);
	REQUIRE(rot0->get_height() == 100.0);

	INFO("Set height");
	rot0->set_height(300.0);
	REQUIRE(rot0->get_width() == 200.0);
	REQUIRE(rot0->get_height() == 300.0);

	INFO("Set width");
	rot0->set_width(40.0);
	REQUIRE(rot0->get_width() == 40.0);
	REQUIRE(rot0->get_height() == 300.0);

	INFO("Rotate after set_height and set_width with constructor");
	auto rot1 = std::make_unique<Rotated>(std::move(rot0), Rotated::QUARTER);
	REQUIRE(rot1->get_width() == 300.0);
	REQUIRE(rot1->get_height() == 40.0);
}

TEST_CASE("Scaled set/get", "[set/get]")
{
	auto rect0 = std::make_unique<Rectangle>(100.0, 200.0);
	auto rect1 = std::make_unique<Rectangle>(100.0, 200.0);

	INFO("Constructor creation 10 times width and height");
	auto scale0 = std::make_unique<Scaled>(std::move(rect0), 10, 10);
	REQUIRE(scale0->get_width() == 1000.0);
	REQUIRE(scale0->get_height() == 2000.0);

	INFO("Constructor creation 0.5 times width and height");
	auto scale1 = std::make_unique<Scaled>(std::move(rect1), 0.5, 0.5);
	REQUIRE(scale1->get_width() == 50.0);
	REQUIRE(scale1->get_height() == 100.0);

	INFO("Set height");
	scale1->set_height(200.0);
	REQUIRE(scale1->get_width() == 50.0);
	REQUIRE(scale1->get_height() == 200.0);

	INFO("Set width");
	scale1->set_width(200.0);
	REQUIRE(scale1->get_width() == 200.0);
	REQUIRE(scale1->get_height() == 200.0);
}

TEST_CASE("Layered set/get", "[set/get]")
{
	auto rect0 = std::make_unique<Rectangle>(100.0, 200.0);
	auto rect1 = std::make_unique<Rectangle>(200.0, 100.0);
	std::initializer_list<std::shared_ptr<Shape>> list0 = { std::move(rect0), std::move(rect1) };

	INFO("Constructor creation with double overlapping rectangles");
	auto lay0 = std::make_unique<Layered>(list0);
	REQUIRE(lay0->get_width() == 200.0);
	REQUIRE(lay0->get_height() == 200.0);

	auto rect2 = std::make_unique<Rectangle>(400.0, 100.0);
	auto circle0 = std::make_unique<Circle>(150.0);
	std::initializer_list<std::shared_ptr<Shape>> list1 = { std::move(rect2), std::move(circle0) };

	INFO("Constructor creation with circle overlapping square");
	auto lay1 = std::make_unique<Layered>(list1);
	REQUIRE(lay1->get_width() == 400.0);
	REQUIRE(lay1->get_height() == 300.0);

	INFO("Set height");
	lay1->set_height(200.0);
	REQUIRE(lay1->get_width() == 400.0);
	REQUIRE(lay1->get_height() == 200.0);

	INFO("Set width");
	lay1->set_width(1200.0);
	REQUIRE(lay1->get_width() == 1200.0);
	REQUIRE(lay1->get_height() == 200.0);
}

TEST_CASE("Vertical set/get", "[set/get]")
{
	auto circle = std::make_unique<Circle>(3.);
	auto rectangle = std::make_unique<Rectangle>(40,50);
	auto spacer = std::make_unique<Spacer>(100,100);
	auto test_verticle = std::make_unique<Virtical>(std::initializer_list<std::shared_ptr<Shape>>{std::move(circle), std::move(spacer), std::move(rectangle)});
	REQUIRE(test_verticle->get_height() == 156.0);
}

TEST_CASE("Horizontal set/get", "[set/get]")
{
	auto circle = std::make_unique<Circle>(3.);
	auto rectangle = std::make_unique<Rectangle>(40,50);
	auto spacer = std::make_unique<Spacer>(100,100);
	auto test_horizontal = std::make_unique<Horizontal>(std::initializer_list<std::shared_ptr<Shape>>{std::move(circle), std::move(spacer), std::move(rectangle)});
	REQUIRE(test_horizontal->get_width() == 146.0);
}

TEST_CASE("Layered Rotated set/get", "[set/get]")
{
	auto square = std::make_unique<Square>(30.0);
	auto rectangle = std::make_unique<Rectangle>(40, 50);
	auto spacer = std::make_unique<Spacer>(20, 60);

	auto rotated_rectangle = std::make_unique<Rotated>(std::move(rectangle), Rotated::QUARTER);
	auto rotated_square = std::make_unique<Rotated>(std::move(square), Rotated::QUARTER);

	auto test_layered_rotated = std::make_unique<Layered>(std::initializer_list<std::shared_ptr<Shape>>{std::move(rotated_square), std::move(spacer), std::move(rotated_rectangle)});
	REQUIRE(test_layered_rotated->get_height() == 60.0);
	REQUIRE(test_layered_rotated->get_width() == 50.0);
}

TEST_CASE("Vertical Rotated set/get", "[set/get]")
{
	auto square = std::make_unique<Square>(50.0);
	auto rectangle = std::make_unique<Rectangle>(40,50);
	auto spacer = std::make_unique<Spacer>(100,100);

	auto rotated_rectangle = std::make_unique<Rotated>(std::move(rectangle), Rotated::QUARTER);
	auto rotated_square = std::make_unique<Rotated>(std::move(square), Rotated::QUARTER);

	auto test_verticle_rotated = std::make_unique<Virtical>(std::initializer_list<std::shared_ptr<Shape>>{std::move(rotated_square), std::move(spacer), std::move(rotated_rectangle)});
	REQUIRE(test_verticle_rotated->get_height() == 190.0);
}

TEST_CASE("Horizontal Rotated set/get", "[set/get]")
{
	auto square = std::make_unique<Square>(50.0);
	auto rectangle = std::make_unique<Rectangle>(40,50);
	auto spacer = std::make_unique<Spacer>(100,100);

	auto rotated_rectangle = std::make_unique<Rotated>(std::move(rectangle), Rotated::QUARTER);
	auto rotated_square = std::make_unique<Rotated>(std::move(square), Rotated::QUARTER);

	auto test_verticle_rotated = std::make_unique<Horizontal>(std::initializer_list<std::shared_ptr<Shape>>{std::move(rotated_square), std::move(spacer), std::move(rotated_rectangle)});
	REQUIRE(test_verticle_rotated->get_width() == 200.0);
}

TEST_CASE("Layered Scaled set/get", "[set/get]")
{
	auto square = std::make_unique<Square>(10.);
	auto rect = std::make_unique<Rectangle>(20., 40.);
	auto spacer = std::make_unique<Spacer>(30., 40.);

	auto ssquare = std::make_unique<Scaled>(std::move(square), 2., 2.);
	auto srect = std::make_unique<Scaled>(std::move(rect), 4., 4.);
	auto sspacer = std::make_unique<Scaled>(std::move(spacer), 3., 3.);


	auto layered_scaled = std::make_unique<Layered>(std::initializer_list < std::shared_ptr<Shape>>{std::move(ssquare), std::move(sspacer), std::move(srect)});
	REQUIRE(layered_scaled->get_width() == 90.);
	REQUIRE(layered_scaled->get_height() == 160.);
}

TEST_CASE("Vertical Scaled set/get", "[set/get]")
{
	auto square = std::make_unique<Square>(10.);
	auto rect = std::make_unique<Rectangle>(20., 40.);
	auto spacer = std::make_unique<Spacer>(30., 40.);

	auto ssquare = std::make_unique<Scaled>(std::move(square), 2., 2.);
	auto srect = std::make_unique<Scaled>(std::move(rect), 4., 4.);
	auto sspacer = std::make_unique<Scaled>(std::move(spacer), 3., 3.);


	auto vertical_scaled = std::make_unique<Virtical>(std::initializer_list < std::shared_ptr<Shape>>{std::move(ssquare), std::move(sspacer), std::move(srect)});
	REQUIRE(vertical_scaled->get_width() == 90.);
	REQUIRE(vertical_scaled->get_height() == 300.);
}

TEST_CASE("Horizontal Scaled set/get", "[set/get]")
{
	auto square = std::make_unique<Square>(10.);
	auto rect = std::make_unique<Rectangle>(20., 40.);
	auto spacer = std::make_unique<Spacer>(30., 40.);

	auto ssquare = std::make_unique<Scaled>(std::move(square), 2., 2.);
	auto srect = std::make_unique<Scaled>(std::move(rect), 4., 4.);
	auto sspacer = std::make_unique<Scaled>(std::move(spacer), 3., 3.);


	auto horiz_scaled = std::make_unique<Horizontal>(std::initializer_list < std::shared_ptr<Shape>>{std::move(ssquare), std::move(sspacer), std::move(srect)});
	REQUIRE(horiz_scaled->get_width() == 190.);
	REQUIRE(horiz_scaled->get_height() == 160.);
}

TEST_CASE("Horizontal in Vertical Shape", "[set/get]")
{
	auto s1 = std::make_unique<Square>(50.);
	auto s2 = std::make_unique<Square>(40.);
	auto s3 = std::make_unique<Square>(20.);
	auto s4 = std::make_unique<Square>(10.);
	auto horiz1 = std::make_unique<Horizontal>(std::initializer_list<std::shared_ptr<Shape>>{std::move(s1), std::move(s2), std::move(s3), std::move(s4)});

	auto rect = std::make_unique<Rectangle>(40., 20.);
	auto rect2= std::make_unique<Rectangle>(40., 20.);
	
	auto horiz2 = std::make_unique<Horizontal>(std::initializer_list<std::shared_ptr<Shape>>{std::move(horiz1), std::move(rect)});

	auto s5 = std::make_unique<Square>(50.);
	auto s6 = std::make_unique<Square>(40.);
	auto s7 = std::make_unique<Square>(20.);
	auto s8 = std::make_unique<Square>(10.);
	auto horiz3 = std::make_unique<Horizontal>(std::initializer_list<std::shared_ptr<Shape>>{std::move(s5), std::move(s6), std::move(s7), std::move(s8)});

	auto vertical = std::make_unique<Virtical>(std::initializer_list<std::shared_ptr<Shape>>{std::move(horiz2), std::move(horiz3), std::move(rect2)});

	REQUIRE(vertical->get_width() == 160.);
	REQUIRE((int)(vertical->get_height()) == 120);
}

TEST_CASE("Vertical in Horizontal Shape", "[set/get]")
{
	auto s1 = std::make_unique<Square>(50.);
	auto s2 = std::make_unique<Square>(40.);
	auto s3 = std::make_unique<Square>(20.);
	auto s4 = std::make_unique<Square>(10.);
	auto vert1 = std::make_unique<Virtical>(std::initializer_list<std::shared_ptr<Shape>>{std::move(s1), std::move(s2), std::move(s3), std::move(s4)});

	auto rect = std::make_unique<Rectangle>(40., 20.);
	auto rect2 = std::make_unique<Rectangle>(40., 20.);

	auto vert2 = std::make_unique<Virtical>(std::initializer_list<std::shared_ptr<Shape>>{std::move(vert1), std::move(rect)});

	auto s5 = std::make_unique<Square>(50.);
	auto s6 = std::make_unique<Square>(40.);
	auto s7 = std::make_unique<Square>(20.);
	auto s8 = std::make_unique<Square>(10.);
	auto vert3 = std::make_unique<Virtical>(std::initializer_list<std::shared_ptr<Shape>>{std::move(s5), std::move(s6), std::move(s7), std::move(s8)});

	auto horiz = std::make_unique<Horizontal>(std::initializer_list<std::shared_ptr<Shape>>{std::move(vert2), std::move(vert3), std::move(rect2)});

	REQUIRE(horiz->get_width() == 140.);
	REQUIRE((int)(horiz->get_height()) == 140);
}


TEST_CASE("Extra Shapes, Sierpinskis triange", "[set/get]")
{
	INFO("Constructor");
	auto stri = std::make_unique<STriangle>(50., 2);
	auto tri = std::make_unique<Triangle>(50.);
	REQUIRE(stri->get_height() == tri->get_height());
	REQUIRE(stri->get_width() == tri->get_width());

}

TEST_CASE("Extra Shapes, Diamond", "[set/get]")
{
	INFO("Constructor");
	auto diamond = std::make_unique<Diamond>(50.);
	REQUIRE(diamond->get_height() == (50.*sqrt(3.)));
	REQUIRE(diamond->get_width() == 50.);
}
