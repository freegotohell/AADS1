#include "grayscale.h"
#include <iostream>
#include <stdexcept>

int main() {
	try {

		short scalar = 10;

		Grayscale<short> img1(3, 3, true);
		Grayscale<short> img2(3, 3, true);

		auto sum_1 = img1 + img2;
		auto sum_2 = scalar + img1;

		auto diff_1 = img1 - img2;
		auto diff_2 = img1 - scalar;
		auto diff_3 = scalar - img1;


		auto product_1 = img1 * img2;
		auto product_2 = img1 * 10;

		std::cout << "image 1\n" << img1;
		std::cout << "\nimage 2\n" << img2;
		std::cout << "\nsum of images\n" << sum_1;
		std::cout << "\nsum of image 1 and number\n" << sum_2;
		std::cout << "\ndifference of images\n" << diff_1;
		std::cout << "\ndifference of image 1 and number\n" << diff_2;
		std::cout << "\ndifference of number and image 1\n" << diff_3;
		std::cout << "\nproduct of images\n" << product_1;
		std::cout << "\nproduct of number and image 1\n" << product_2;
		std::cout << "\ninverted image 1\n" << !img1;
		std::cout << "\nfill coefficient of image 1\n" << img1.fill_coefficient() << '\n';


		Grayscale<short> img3(10, 10, false);
		short fill = 0;

		size_t x1, y1;
		size_t x2, y2;

		
		std::cout << "\nenter top left point ";
		std::cin >> x1 >> y1;

		std::cout << "\nenter bottom right point ";
		std::cin >> x2 >> y2;

		std::cout << "\nenter number for filling ";
		std::cin >> fill;

		draw_rectangle(img3, x1, y1, x2, y2, fill);

		std::cout << img3;
	}
	catch (const std::invalid_argument& e) {
		std::cerr << e.what() << "\n";
	}
	catch (const std::out_of_range& e) {
		std::cerr << e.what() << "\n";
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << "\n";
	}

	return 0;
}