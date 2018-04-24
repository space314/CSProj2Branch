// shape.h
// Project: CPS
// Base class Shape 

#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <fstream>
#include <iostream>

#include <initializer_list>
#include <exception>

#ifndef __SHAPES_HPP__
#define __SHAPES_HPP__

class Shape {
public:
    Shape(double width, double height);
    virtual ~Shape() = default;
    virtual std::string to_postscript() const = 0;
	double get_width() const;
	double get_height() const;
	void set_width(double width);
	void set_height(double height);
	void generate_postscript_file(std::string file_name);
private:
    double m_width;
    double m_height;
};

class Circle: public Shape
{
public:
	Circle(double radius);
	~Circle() = default;
	std::string to_postscript() const;
private:
	double m_radius;
};

class Polygon : public Shape
{
public:
	Polygon(int num_sides, double side_length);
	virtual ~Polygon() = default;
	std::string to_postscript() const;
private:
	int m_num_sides;
	double m_side_length;
};


class Rectangle : public Shape
{
public:
	Rectangle(double width, double height): Shape(width, height){}
	~Rectangle() = default;
	std::string to_postscript() const;
private:
};

class Spacer : public Shape
{
public:
	Spacer(double width, double height): Shape(width, height){}
	~Spacer() = default;
	std::string to_postscript() const;
private:
};

class Square : public Polygon
{
public:
	Square(double side_length): Polygon(4, side_length){}
	~Square() = default;
private:
};

class Triangle : public Polygon
{
public:
	Triangle(double side_length): Polygon(3, side_length){}
	~Triangle() = default;
private:
};

class Rotated : public Shape
{
public:
	enum RotationAngle { QUARTER = 90, HALF = 180, THREE_QUARTER = 270 };
	Rotated(std::shared_ptr<Shape> shape, RotationAngle rotation_angle);
	~Rotated() = default;
	std::string to_postscript() const;
private:
	RotationAngle m_rot;
	std::shared_ptr<Shape> m_shape;
};

class Scaled : public Shape
{
public:
	Scaled(std::shared_ptr<Shape> shape, double fx, double fy);
	~Scaled() = default;
	std::string to_postscript() const;
private:
	std::shared_ptr<Shape> m_shape;
	double m_fx;
	double m_fy;
};


template<class Derived>
class Compound : public Shape
{
public:
	using Shape::Shape;
	virtual std::string to_postscript() const override;
	virtual std::string drawShift(double shift, int i) const = 0;
	virtual int get_list_size() const = 0;
	virtual int get_shift(int i) const = 0;
	virtual std::string drawShape(int i) const = 0;
private:
	std::shared_ptr<Shape> m_compound;
};

template<class Derived>
std::string Compound<Derived>::to_postscript() const
{
	std::string outputString = "";
	double total = 0;
	int size = static_cast<const Derived*>(this)->get_list_size();
	for (unsigned int i = 0; i < size; ++i)
	{
		outputString += "gsave\n";
		outputString += static_cast<const Derived*>(this)->drawShift(total, i);
		outputString += static_cast<const Derived*>(this)->drawShape(i);
		outputString += "grestore\n";
		total += static_cast<const Derived*>(this)->get_shift(i);
	}
	return outputString;
}

class Layered : public Compound<Layered>
{
public:
	Layered(std::initializer_list<std::shared_ptr<Shape>> shapes);
	~Layered() = default;
	std::string drawShift(double shift, int i) const;
	int get_list_size() const;
	std::string drawShape(int i) const;
	int get_shift(int i) const;
private:
	std::vector<std::shared_ptr<Shape>> m_shapes;
};

class Virtical : public Compound<Virtical>
{
public:
	Virtical(std::initializer_list<std::shared_ptr<Shape>> shapes);
	~Virtical() = default;
	std::string drawShift(double shift, int i) const;
	int get_list_size() const;
	std::string drawShape(int i) const;
	int get_shift(int i) const;
private:
	std::vector<std::shared_ptr<Shape>> m_shapes;
};

class Horizontal : public Compound<Horizontal>
{
public:
	Horizontal(std::initializer_list<std::shared_ptr<Shape>> shapes);
	~Horizontal() = default;
	std::string drawShift(double shift, int i) const;
	int get_list_size() const;
	std::string drawShape(int i) const;
	int get_shift(int i) const;
private:
	std::vector<std::shared_ptr<Shape>> m_shapes;
};

class Diamond : public Shape
{
public:
	Diamond(double side_length);
	~Diamond() = default;
	std::string to_postscript() const;
private:
};

class STriangle : public Shape
{
public:
	STriangle(double side, int depth);
	~STriangle() = default;
	std::string to_postscript() const;
private:
	std::vector < std::shared_ptr<Shape>> m_subTriangles;
};

class U_Curve: public Shape
{
public:
	U_Curve(double side):Shape(side, side){}
	~U_Curve() = default;
	std::string to_postscript() const;
private:
	std::vector<std::shared_ptr<Shape>> m_subLPB;
};
//Line plane bijection
class LPB : public Shape
{
public:
	LPB(double side, int depth);
	~LPB() = default;
	std::string to_postscript() const;
private:
	std::vector<std::shared_ptr<Shape>> m_subLPB;
	int m_depth;
};
#endif