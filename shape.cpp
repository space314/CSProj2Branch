#include "shape.hpp"
# define M_PI  3.14159265358979323846


Shape::Shape(double width, double height):m_width(width), m_height(height){}

double Shape::get_width() const { return m_width; }
double Shape::get_height() const { return m_height; }

void Shape::set_width(double width) { m_width = width; }
void Shape::set_height(double height) { m_height = height; }

void Shape::generate_postscript_file(std::string file_name)
{
	std::string output_postscript = std::to_string(get_width() / 2 + 36) + " " +
		std::to_string(get_height() / 2 + 36) + " translate\n\n" +
		to_postscript() + "\nshowpage";
	std::fstream open_file(file_name + ".ps", std::fstream::trunc | std::fstream::in | std::fstream::out);

	if (!open_file.is_open()) 
	{
		std::cout << "file failed to open " << file_name + ".ps" << std::endl;
	}
	else
	{
		open_file << output_postscript;
	}
	open_file.close();
}

Circle::Circle(double radius)
	:m_radius(radius), Shape(2 * radius, 2 * radius){}

std::string Circle::to_postscript() const
{
	return "gsave\n"
		"newpath \n"
		"  0 0 " + std::to_string(m_radius) + " 0 360 arc \n"
		"  stroke \n"
		"closepath\n"
		"grestore\n";
}

Polygon::Polygon(int num_sides, double side_length)
	:m_num_sides(num_sides), m_side_length(side_length), Shape(0, 0)
{
	if(num_sides < 3)
	{
		throw std::invalid_argument("a polygon cannot have less than 3 sides");
	}
	double width;
	double height;
	double cos_part = std::cos(M_PI / ((double)num_sides));
	double sin_part = std::sin(M_PI / ((double)num_sides));
	if (num_sides % 2 == 1)
	{
		height = side_length*(1 + cos_part) / (2 * sin_part);
		width = (side_length * std::sin(M_PI* (num_sides - 1) / (double)(2 * num_sides))) / (sin_part);
	}
	else if (num_sides % 4 == 0)
	{
		height = side_length*(cos_part) / (sin_part);
		width = (side_length* cos_part) / (sin_part);
	}
	else
	{
		height = side_length*(cos_part) / (sin_part);
		width = side_length / (sin_part);
	}
	set_width(width);
	set_height(height);
}

std::string Polygon::to_postscript() const
{
	double y_shift = m_side_length/(2*std::tan(M_PI/(double)m_num_sides));
	double x_shift = m_side_length/2;
	return "gsave\n"
		"0 " + std::to_string(-get_height() / 2 + y_shift) + " translate\n"
		"0 " + std::to_string(360 / (double)m_num_sides) + " 360 {\n"
		"  newpath\n"
		"  gsave\n"
		"    rotate\n"
		"    -" + std::to_string(x_shift) + " -" + std::to_string(y_shift) + " moveto\n"
		"    " + std::to_string(x_shift) + " -" + std::to_string(y_shift) + " lineto\n"
		"    stroke\n"
		"  grestore\n"
		"  closepath\n"
		"}for\n"
		"grestore\n";
}

std::string Rectangle::to_postscript() const
{
	return "gsave\n"
		"  newpath\n"
		"  -" + std::to_string(get_width() / 2) + " -" + std::to_string(get_height() / 2) + " moveto\n"
		"  " + std::to_string(get_width() / 2) + " -" + std::to_string(get_height() / 2) + " lineto\n"
		"  " + std::to_string(get_width() / 2) + " -" + std::to_string(get_height() / 2) + " moveto\n"
		"  " + std::to_string(get_width() / 2) + " " + std::to_string(get_height() / 2) + " lineto\n"
		"  " + std::to_string(get_width() / 2) + " " + std::to_string(get_height() / 2) + " moveto\n"
		"  -" + std::to_string(get_width() / 2) + " " + std::to_string(get_height() / 2) + " lineto\n"
		"  -" + std::to_string(get_width() / 2) + " " + std::to_string(get_height() / 2) + " moveto\n"
		"  -" + std::to_string(get_width() / 2) + " -" + std::to_string(get_height() / 2) + " lineto\n"
		"  closepath\n"
		"  stroke\n"
		"grestore\n";
}

std::string Spacer::to_postscript() const
{
	return "";
}

Rotated::Rotated(std::shared_ptr<Shape> shape, RotationAngle rotation_angle)
	:Shape(0,0), m_rot{rotation_angle}, m_shape{std::move(shape)}
{
	if (rotation_angle == HALF)
	{
		set_width(m_shape->get_width());
		set_height(m_shape->get_height());
	}
	else
	{
		set_width(m_shape->get_height());
		set_height(m_shape->get_width());
	}
}

std::string Rotated::to_postscript() const
{
	return "gsave\n"
		+ std::to_string(m_rot) + " rotate\n"
		+ m_shape->to_postscript()
		+ "-" + std::to_string(m_rot) + " rotate\n"
		"grestore\n";
}

Scaled::Scaled(std::shared_ptr<Shape> shape, double fx, double fy)
	:Shape(0,0), m_shape{std::move(shape)}, m_fx{fx}, m_fy{fy}
{
	{
		set_width(m_shape->get_width()*fx);
		set_height(m_shape->get_height()*fy);
	}
}

std::string Scaled::to_postscript() const
{
	return "gsave\n"
		+ std::to_string(m_fx) + " " + std::to_string(m_fy) + " scale\n"
		+ m_shape->to_postscript() +
		"grestore\n";
}

Layered::Layered(std::initializer_list<std::shared_ptr<Shape>> shapes)
	:Shape(0,0), m_shapes(std::move(shapes))
{
	for(unsigned int i = 0; i < m_shapes.size(); ++i)
	{
		if(get_width() < m_shapes[i]->get_width())
			set_width(m_shapes[i]->get_width());
		if(get_height() < m_shapes[i]->get_height())
			set_height(m_shapes[i]->get_height());
	}
}

std::string Layered::to_postscript() const
{
	std::cout << get_width() << std::endl;
	std::cout << get_height() << std::endl;
	std::string outputString = "";
	for(unsigned int i = 0; i < m_shapes.size(); ++i)
	{
		outputString += "gsave\n" +
			m_shapes[i]->to_postscript() +
			"grestore\n";
	}
	return outputString;
}

Virtical::Virtical(std::initializer_list<std::shared_ptr<Shape>> shapes)
	:Shape(0,0), m_shapes(std::move(shapes))
{
	double total_height = 0;
	for(unsigned int i = 0; i < m_shapes.size(); ++i)
	{
		if(get_width() < m_shapes[i]->get_width())
			set_width(m_shapes[i]->get_width());
		total_height += m_shapes[i]->get_height();
	}
	set_height(total_height);
}

std::string Virtical::to_postscript() const
{
	std::string outputString = "";
	double total_height_drawn = 0;
	for(unsigned int i = 0; i < m_shapes.size(); ++i)
	{
		outputString += "gsave\n" 
		"0 " + std::to_string(-get_height()/2 + m_shapes[i]->get_height()/2+total_height_drawn) + " translate\n" +
		m_shapes[i]->to_postscript() +
		"grestore\n";
		total_height_drawn += m_shapes[i]->get_height();
	}
	return outputString;
}

Horizontal::Horizontal(std::initializer_list<std::shared_ptr<Shape>> shapes)
	:Shape(0,0), m_shapes(std::move(shapes))
{
	double total_width = 0;
	for(unsigned int i = 0; i < m_shapes.size(); ++i)
	{
		if(get_height() < m_shapes[i]->get_height())
			set_height(m_shapes[i]->get_height());
		total_width += m_shapes[i]->get_width();
	}
	set_width(total_width);
}

std::string Horizontal::to_postscript() const
{
	std::string outputString = "";
	double total_width_drawn = 0;
	for(unsigned int i = 0; i < m_shapes.size(); ++i)
	{
		outputString += "gsave\n" +
		std::to_string(-get_width()/2 + m_shapes[i]->get_width()/2+total_width_drawn) + " 0" + " translate\n" +
		m_shapes[i]->to_postscript() +
		"grestore\n";
		total_width_drawn += m_shapes[i]->get_width();
	}
	return outputString;
}

STriangle::STriangle(double side_length, int depth):Shape(0,0)
{
	double width;
	double height;
	double cos_part = std::cos(M_PI / ((double)3));
	double sin_part = std::sin(M_PI / ((double)3));
	height = side_length*(1 + cos_part) / (2 * sin_part);
	width = (side_length * std::sin(M_PI* (3 - 1) / (double)(2 * 3))) / (sin_part);
	set_width(width);
	set_height(height);

	if (depth == 0)
		return;

	if (depth == 1)
	{
		auto tri1 = std::make_unique<Triangle>(side_length / 2);
		auto tri2 = std::make_unique<Triangle>(side_length / 2);
		auto tri3 = std::make_unique<Triangle>(side_length / 2);
		m_subTriangles.push_back(std::move(tri1));
		m_subTriangles.push_back(std::move(tri2));
		m_subTriangles.push_back(std::move(tri3));
	}
	else
	{
		auto tri1 = std::make_unique<STriangle>(side_length / 2, depth - 1);
		auto tri2 = std::make_unique<STriangle>(side_length / 2, depth - 1);
		auto tri3 = std::make_unique<STriangle>(side_length / 2, depth - 1);
		m_subTriangles.push_back(std::move(tri1));
		m_subTriangles.push_back(std::move(tri2));
		m_subTriangles.push_back(std::move(tri3));
	}
}

std::string STriangle::to_postscript() const
{
	if (m_subTriangles.size() == 0)
	{
		auto tri = std::make_unique<Triangle>(get_width());
		return tri->to_postscript();
	}

	return "gsave\n"
			"-" + std::to_string(get_width() / 4) + " -" + std::to_string(get_height() / 4) + " translate\n"
			+ m_subTriangles.at(0)->to_postscript() +
			std::to_string(get_width() / 4) + " " + std::to_string(get_height() / 2) + " translate\n"
			+ m_subTriangles.at(1)->to_postscript() +
			std::to_string(get_width() / 4) + " -" + std::to_string(get_height() / 2) + " translate\n"
			+ m_subTriangles.at(2)->to_postscript()+
			"grestore\n";
}
Diamond::Diamond(double side_length)
	:Shape(0, 0)
{
	set_width(side_length);
	set_height(std::sqrt(3)*side_length);
}

std::string Diamond::to_postscript() const
{
	return "gsave\n"
		"  newpath\n"
		"  -" + std::to_string(get_width()/2) + " -" + std::to_string(0) + " moveto\n"
		"  -" + std::to_string(0) + " -" + std::to_string(get_height()/2) + " lineto\n"
		"  -" + std::to_string(0) + " -" + std::to_string(get_height()/2) + " moveto\n"
		"  " + std::to_string(get_width()/2) + " " + std::to_string(0) + " lineto\n"
		"  " + std::to_string(get_width()/2) + " " + std::to_string(0) + " moveto\n"
		"  " + std::to_string(0) + " " + std::to_string(get_height()/2) + " lineto\n"
		"  " + std::to_string(0) + " " + std::to_string(get_height()/2) + " moveto\n"
		"  -" + std::to_string(get_width()/2) + " -" + std::to_string(0) + " lineto\n"
		"  closepath\n"
		"  stroke\n"
		"grestore\n";
}

LPB::LPB(double side, int depth): Shape(side, side), m_depth(depth)
{
	if (depth == 1)
	{
		auto q1 = std::make_unique<U_Curve>(side / 2);
		auto q2 = std::make_unique<U_Curve>(side / 2);
		auto q3 = std::make_unique<U_Curve>(side / 2);
		auto q4 = std::make_unique<U_Curve>(side / 2);
		m_subLPB.push_back(std::move(q1));
		m_subLPB.push_back(std::move(q2));
		m_subLPB.push_back(std::move(q3));
		m_subLPB.push_back(std::move(q4));
	}
	else
	{
		auto q1 = std::make_unique<LPB>(side / 2, depth-1);
		auto q2 = std::make_unique<LPB>(side / 2, depth-1);
		auto q3 = std::make_unique<LPB>(side / 2, depth-1);
		auto q4 = std::make_unique<LPB>(side / 2, depth-1);
		m_subLPB.push_back(std::move(q1));
		m_subLPB.push_back(std::move(q2));
		m_subLPB.push_back(std::move(q3));
		m_subLPB.push_back(std::move(q4));

	}
}

std::string LPB::to_postscript() const
{
	if (m_subLPB.size() == 0)
	{
		auto shape = std::make_unique<U_Curve>(get_width());
		return shape->to_postscript();
	}
	return "gsave\n"
		"-" + std::to_string(get_width() / 4) + " -" + std::to_string(get_height() / 4) + " translate\n"
		"-90 rotate\n"
		+ m_subLPB.at(0)->to_postscript() +
		+"90 rotate\n"
		"0 " + std::to_string(get_height() / 2) + " translate\n"
		+ m_subLPB.at(1)->to_postscript() +
		std::to_string(get_width() / 2) + " 0 translate\n"
		+ m_subLPB.at(2)->to_postscript() +
		+"0 -" + std::to_string(get_height() / 2) + " translate\n"
		+ "90 rotate\n"
		+ m_subLPB.at(3)->to_postscript() +
		"-90 rotate\n"
		"grestore\n"
		"gsave\n"
		"newpath\n"
		"-" + std::to_string(get_width() / 2 - get_width() / (4 * pow(2, m_depth))) +
		" -" + std::to_string(get_width() / (4 * pow(2, m_depth))) + " moveto\n"
		"0 " + std::to_string(get_width() / (4 * pow(2, m_depth - 1))) + " rlineto\n"
		"stroke\n"
		"newpath\n"
		"-" + std::to_string(get_width() / (4 * pow(2, m_depth))) +
		" " + std::to_string(get_width() / (4 * pow(2, m_depth))) + " moveto\n"
		 + std::to_string(get_width() / (4 * pow(2, m_depth - 1))) + " 0 rlineto\n"
		"stroke\n"
		"newpath\n"
		"" + std::to_string(get_width()/2 - get_width() / (4 * pow(2, m_depth))) +
		" " + std::to_string(get_width() / (4 * pow(2, m_depth))) + " moveto\n"
		"0 -" + std::to_string(get_width() / (4 * pow(2, m_depth - 1))) + " rlineto\n"
		"stroke\n"
		"grestore\n";

}

std::string U_Curve::to_postscript() const
{
	return "gsave\n"
		"newpath\n"
		"-" + std::to_string(get_width() / 4) + " -" + std::to_string(get_height() / 4) + " moveto\n"
		"0 " + std::to_string(get_height() / 2) + " rlineto\n"
		+ std::to_string(get_width() / 2) + " 0 rlineto\n"
		"0 -" + std::to_string(get_height() / 2) + " rlineto\n"
		"stroke\n"
		"grestore\n";
}
