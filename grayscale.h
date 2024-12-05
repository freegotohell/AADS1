#include <iostream>
#include <random>
#include <limits>
#include <cmath>

template <typename T>
class Grayscale {
private:
    size_t _width;
    size_t _height;
    T* _data;
public:
	static constexpr double epsilon = 1e-6;

	Grayscale<T>::Grayscale(size_t height, size_t width, bool fill_random = true) : _height(height), _width(width) {

		_data = new T[height * width];

		std::random_device rd;
		std::mt19937 gen(rd());

		if (fill_random) {

			if (std::is_same_v<T, float>) {
				for (size_t i = 0; i < width * height; ++i) {
					std::uniform_real_distribution<> dist(0.0, 255.0);
					_data[i] = static_cast<T>(dist(gen));
				}
			}
			else if (std::is_same_v<T, bool>) {
				for (size_t i = 0; i < width * height; ++i) {
					std::uniform_int_distribution<> dist(0, 1);
					_data[i] = static_cast<T>(dist(gen));
				}
			}
			else if (std::is_same_v<T, short> || std::is_same_v<T, char>) {
				for (size_t i = 0; i < width * height; ++i) {
					std::uniform_int_distribution<> dist(0, 255);
					_data[i] = static_cast<T>(dist(gen));
				}
			}
		}
		else {
			for (size_t i = 0; i < width * height; ++i) {
				_data[i] = 0;
			}
		}
	}

	Grayscale(const Grayscale& other) {

		_height = other._height;
		_width = other._width;
		_data = new T[_width * _height];

		for (size_t i = 0; i < _width * _height; ++i) {
			_data[i] = other._data[i];
		}
	}

	Grayscale& operator=(const Grayscale& other) {
		if (this != &other) {
			delete[] _data;
			_height = other._height;
			_width = other._width;
			_data = new T[_width * _height];

			for (size_t i = 0; i < _width * _height; ++i) {
				_data[i] = other._data[i];
			}
		}
		return *this;
	}

	~Grayscale() {
		delete[] _data;
	}

	T& operator()(size_t x, size_t y) {
		if (x >= _width || y >= _height) {
			throw std::out_of_range("index out of range");
		}
		return _data[y * _width + x];
	}

	const T& operator()(size_t x, size_t y) const {
		if (x >= _width || y >= _height) {
			throw std::out_of_range("index out of range");
		}
		return _data[y * _width + x];
	}

	Grayscale operator*(const Grayscale& other) const {
		if (_width != other._width || _height != other._height) {
			throw std::invalid_argument("sizes don't match");
		}

		T max = std::numeric_limits<T>::max();
		T min = std::numeric_limits<T>::min();

		Grayscale<T> res(_width, _height);

		for (size_t i = 0; i < _width * _height; ++i) {

			if (_data[i] != 0 && other._data[i] > max / _data[i]) {
				res._data[i] = max;
			}
			else if (_data[i] != 0 && other._data[i] < min / _data[i]) {
				res._data[i] = min;
			}
			else {
				res._data[i] = _data[i] * other._data[i];
			}
		}
		return res;
	}

	Grayscale operator*(T scalar) {

		T max = std::numeric_limits<T>::max();
		T min = std::numeric_limits<T>::min();

		Grayscale<T> res(_width, _height);

		for (size_t i = 0; i < _width * _height; ++i) {
			if (_data[i] != 0 && scalar > max / _data[i]) {
				res._data[i] = max;
			}
			else if (_data[i] != 0 && scalar < min / _data[i]) {
				res._data[i] = min;
			}
			else {
				res._data[i] = _data[i] * scalar;
			}
		}
		return res;
	}

	Grayscale& operator*=(T scalar) {

		T max = std::numeric_limits<T>::max();
		T min = std::numeric_limits<T>::min();

		for (size_t i = 0; i < _width * _height; ++i) {
			if (_data[i] != 0 && scalar > max / _data[i]) {
				_data[i] = max;
			}
			else if (_data[i] != 0 && scalar < min / _data[i]) {
				_data[i] = min;
			}
			else {
				_data[i] *= scalar;
			}
		}
		return *this;
	}

	Grayscale operator+(const Grayscale& other) const {
		size_t width = std::max(_width, other._width);
		size_t height = std::max(_height, other._height);

		T max = std::numeric_limits<T>::max();

		Grayscale<T> res(width, height);

		for (size_t y = 0; y < height; ++y) {
			for (size_t x = 0; x < width; ++x) {
				T px1 = (x < _width && y < _height) ? _data[y * _width + x] : 0;
				T px2 = (x < other._width && y < other._height) ? other._data[y * other._width + x] : 0;

				if (px1 > max - px2) {
					res(x,y) = max;
				}
				else {
					res(x,y) = px1 + px2;
				}
			}
		}
		return res;
	}

	Grayscale operator+(T scalar) {
		T max = std::numeric_limits<T>::max();

		Grayscale<T> res(_width, _height);

		for (size_t i = 0; i < _width * _height; ++i) {
			if (_data[i] > max - scalar) {
				res._data[i] = max;
			}
			else {
				res._data[i] = _data[i] + scalar;
			}
		}
		return res;
	}

	Grayscale& operator+=(T scalar) {
		T max = std::numeric_limits<T>::max();

		for (size_t i = 0; i < _width * _height; ++i) {
			if (_data[i] > max - scalar) {
				_data[i] = max;
			}
			else {
				_data[i] += scalar;
			}
		}
		return *this;
	}

	Grayscale operator-(const Grayscale& other) const {
		size_t width = std::max(_width, other._width);
		size_t height = std::max(_height, other._height);

		T min = std::numeric_limits<T>::min();

		Grayscale<T> res(width, height);

		for (size_t y = 0; y < height; ++y) {
			for (size_t x = 0; x < width; ++x) {
				T px1 = (x < _width && y < _height) ? _data[y * _width + x] : 0;
				T px2 = (x < other._width && y < other._height) ? other._data[y * other._width + x] : 0;

				if (px1 < min + px2) {
					res(x, y) = min;
				}
				else {
					res(x, y) = px1 - px2;
				}
			}
		}
		return res;
	}

	Grayscale operator-(T scalar) {
		T min = std::numeric_limits<T>::min();

		Grayscale<T> res(_width, _height);

		for (size_t i = 0; i < _width * _height; ++i) {
			if (_data[i] < min + scalar) {
				res._data[i] = min;
			}
			else {
				res._data[i] = _data[i] - scalar;
			}
		}
		return res;
	}

	Grayscale operator!() const {
		Grayscale res(_width, _height);
		for (size_t i = 0; i < _width * _height; ++i) {
			if (std::is_same_v<T, bool>) {
				res._data[i] = !_data[i];
			}
			else {
				res._data[i] = _data[i] * (-1);
			}
		}
		return res;
	}

	bool operator==(const Grayscale& other) const {
		if (_width != other._width || _height != other._height) {
			return false;
		}
		for (size_t i = 0; i < _width * _height; ++i) {
			if (std::fabs(_data[i] - other._data[i]) > epsilon) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const Grayscale& other) const {
		return !(*this == other);
	}

	double fill_coefficient() const {
		double sum = 0.0;

		for (size_t i = 0; i < _height * _width; ++i) {
			sum += static_cast<double>(_data[i]);
		}

		return sum / (_height * _width * (double)std::numeric_limits<T>::max());
	}

	size_t get_height() const {
		return _height;
	}

	size_t get_width() const {
		return _width;
	}
};
template <typename T>
void draw_rectangle(Grayscale<short>& img, size_t x1, size_t y1, size_t x2, size_t y2, T fill_value) {
	for (size_t y = std::min(y1, y2); y <= std::max(y1, y2); ++y) {
		for (size_t x = std::min(x1, x2); x <= std::max(x1, x2); ++x) {
			if (x < img.get_width() && y < img.get_height()) {
				img(x, y) = fill_value;
			}
		}
	}
}
template <typename T>
Grayscale<T> operator+(T scalar, Grayscale<T>& img) {
	return img + scalar;
}

template <typename T>
Grayscale<T> operator-(T scalar, Grayscale<T>& img) {
	return !(img-scalar);
}

template <typename T>
 std::ostream& operator<<(std::ostream& os, const Grayscale<T>& img) {
	for (size_t y = 0; y < img.get_height(); ++y) {
		for (size_t x = 0; x < img.get_width(); ++x) {
			os << img(x, y) << " ";
		}
		os << "\n";
	}
	return os;
}