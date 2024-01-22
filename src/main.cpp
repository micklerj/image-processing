#include "Images.h"
#include <algorithm>


char tempc;
short temps;
unsigned char tempu;
int length;

Image read(string filename) {
	Image image;
	ifstream file(filename, ios_base::binary);

	if (file.is_open()) {
		//cout << "reading..." << endl;

		file.read((char*)(&tempc), 1);
		image.idLength = tempc;
		file.read((char*)(&tempc), 1);
		image.colorMapType = tempc;
		file.read((char*)(&tempc), 1);
		image.dataTypeCode = tempc;

		file.read((char*)(&temps), 2);
		image.colorMapOrigin = temps;
		file.read((char*)(&temps), 2);
		image.colorMapLength = temps;
		file.read((char*)(&tempc), 1);
		image.colorMapDepth = tempc;

		file.read((char*)(&temps), 2);
		image.xOrigin = temps;
		file.read((char*)(&temps), 2);
		image.yOrigin = temps;

		file.read((char*)(&temps), 2);    //width
		image.width = temps;
		file.read((char*)(&temps), 2);    //height
		image.height = temps;

		file.read((char*)(&tempc), 1);
		image.bitsPerPixel = tempc;
		file.read((char*)(&tempc), 1);
		image.imageDescriptor = tempc;

		length = int(image.width) * int(image.height);

		//file pixel data:
		for (int i = 0; i < length; ++i) {
			Pixel pixel;
			file.read((char*)(&tempu), 1);  //blue
			pixel.setb(tempu);
			file.read((char*)(&tempu), 1);  //green
			pixel.setg(tempu);
			file.read((char*)(&tempu), 1);  //red
			pixel.setr(tempu);
			image.add_pixel(pixel);
		}

		file.close();
	}
	else {
		cout << "File does not exist." << endl;

	}

	return image;
}

void write(string filepath, Image image) {
	ofstream fileO(filepath, ios_base::binary);

	if (fileO.is_open()) {
		//cout << "writing..." << endl;

		tempc = image.idLength;
		fileO.write((char*)(&tempc), 1);
		tempc = image.colorMapType;
		fileO.write((char*)(&tempc), 1);
		tempc = image.dataTypeCode;
		fileO.write((char*)(&tempc), 1);

		temps = image.colorMapOrigin;
		fileO.write((char*)(&temps), 2);
		temps = image.colorMapLength;
		fileO.write((char*)(&temps), 2);
		tempc = image.colorMapDepth;
		fileO.write((char*)(&tempc), 1);

		temps = image.xOrigin;
		fileO.write((char*)(&temps), 2);
		temps = image.yOrigin;
		fileO.write((char*)(&temps), 2);

		temps = image.width;                //width
		fileO.write((char*)(&temps), 2);
		temps = image.height;              //height
		fileO.write((char*)(&temps), 2);

		tempc = image.bitsPerPixel;
		fileO.write((char*)(&tempc), 1);
		tempc = image.imageDescriptor;
		fileO.write((char*)(&tempc), 1);

		for (Pixel& p : image.get_pixels()) {
			unsigned char b = p.blue;
			fileO.write((char*)&b, 1);
			b = p.green;
			fileO.write((char*)&b, 1);
			b = p.red;
			fileO.write((char*)&b, 1);
		}

		fileO.close();
	}
	else {
		cout << "failed to write the file" << endl;
	}
}

Image multiply(Image top, Image bottom) {
	//cout << "multiplying..." << endl;

	vector <Pixel> topvec = top.get_pixels();
	vector <Pixel> botvec = bottom.get_pixels();

	for (int i = 0; i < length; ++i) {
		unsigned char topblue = (topvec[i]).blue;
		unsigned char topgreen = (topvec[i]).green;
		unsigned char topred = (topvec[i]).red;

		unsigned char botblue = (botvec[i]).blue;
		unsigned char botgreen = (botvec[i]).green;
		unsigned char botred = (botvec[i]).red;

		float topb = float(topblue) / 255;
		float topg = float(topgreen) / 255;
		float topr = float(topred) / 255;
		float botb = float(botblue) / 255;
		float botg = float(botgreen) / 255;
		float botr = float(botred) / 255;

		topb = (topb * botb * 255) + 0.5f;
		topg = (topg * botg * 255) + 0.5f;
		topr = (topr * botr * 255) + 0.5f;

		unsigned char bresult = (unsigned char)topb;
		unsigned char gresult = (unsigned char)topg;
		unsigned char rresult = (unsigned char)topr;

		top.set_pixel(i, bresult, gresult, rresult);
	}

	return top;
}

Image screen(Image top, Image bottom) {
	//cout << "screening..." << endl;

	vector <Pixel> topvec = top.get_pixels();
	vector <Pixel> botvec = bottom.get_pixels();

	for (int i = 0; i < length; ++i) {
		float topb = float((topvec[i]).blue);
		float topg = float((topvec[i]).green);
		float topr = float((topvec[i]).red);

		float botb = float((botvec[i]).blue);
		float botg = float((botvec[i]).green);
		float botr = float((botvec[i]).red);

		topb = 1 - (topb / 255);
		topg = 1 - (topg / 255);
		topr = 1 - (topr / 255);
		botb = 1 - (botb / 255);
		botg = 1 - (botg / 255);
		botr = 1 - (botr / 255);

		topb = (1 - topb * botb) * 255 + 0.5f;
		topg = (1 - topg * botg) * 255 + 0.5f;
		topr = (1 - topr * botr) * 255 + 0.5f;

		unsigned char bresult = (unsigned char)topb;
		unsigned char gresult = (unsigned char)topg;
		unsigned char rresult = (unsigned char)topr;

		top.set_pixel(i, bresult, gresult, rresult);
	}
	return top;
}

Image subtract(Image top, Image bottom) {   //top - bottom
	//cout << "subtracting..." << endl;

	vector <Pixel> topvec = top.get_pixels();
	vector <Pixel> botvec = bottom.get_pixels();

	for (int i = 0; i < length; ++i) {
		int topb = int((topvec[i]).blue);
		int topg = int((topvec[i]).green);
		int topr = int((topvec[i]).red);

		int botb = int((botvec[i]).blue);
		int botg = int((botvec[i]).green);
		int botr = int((botvec[i]).red);

		topb = topb - botb;
		topg = topg - botg;
		topr = topr - botr;

		if (topb < 0) {
			topb = 0;
		}
		if (topg < 0) {
			topg = 0;
		}
		if (topr < 0) {
			topr = 0;
		}
		unsigned char bresult = (unsigned char)topb;
		unsigned char gresult = (unsigned char)topg;
		unsigned char rresult = (unsigned char)topr;

		top.set_pixel(i, bresult, gresult, rresult);
	}

	return top;
}

Image overlay(Image top, Image bottom) {
	//cout << "overlaying..." << endl;

	vector <Pixel> topvec = top.get_pixels();
	vector <Pixel> botvec = bottom.get_pixels();

	for (int i = 0; i < length; ++i) {
		float topb = float((topvec[i]).blue);
		float topg = float((topvec[i]).green);
		float topr = float((topvec[i]).red);

		float botb = float((botvec[i]).blue);
		float botg = float((botvec[i]).green);
		float botr = float((botvec[i]).red);

		topb /= 255;
		topg /= 255;
		topr /= 255;
		botb /= 255;
		botg /= 255;
		botr /= 255;

		if (botb <= .5) {
			topb = topb * botb * 2 * 255 + 0.5f;
		}
		else {
			topb = (1 - 2 * (1 - topb) * (1 - botb)) * 255 + 0.5f;
		}

		if (botg <= .5) {
			topg = topg * botg * 2 * 255 + 0.5f;
		}
		else {
			topg = (1 - 2 * (1 - topg) * (1 - botg)) * 255 + 0.5f;
		}

		if (botr <= .5) {
			topr = topr * botr * 2 * 255 + 0.5f;
		}
		else {
			topr = (1 - 2 * (1 - topr) * (1 - botr)) * 255 + 0.5f;
		}

		//clamping:
		if (topb < 0) {
			topb = 0;
		}
		if (topg < 0) {
			topg = 0;
		}
		if (topr < 0) {
			topr = 0;
		}
		if (topb > 255) {
			topb = 255;
		}
		if (topg > 255) {
			topg = 255;
		}
		if (topr > 255) {
			topr = 255;
		}

		unsigned char bresult = (unsigned char)topb;
		unsigned char gresult = (unsigned char)topg;
		unsigned char rresult = (unsigned char)topr;

		top.set_pixel(i, bresult, gresult, rresult);
	}

	return top;
}

Image addblue(Image top, int num) {
	//cout << "adding to blue..." << endl;

	vector <Pixel> topvec = top.get_pixels();
	for (int i = 0; i < length; ++i) {
		int topb = int((topvec[i]).blue);

		topb += num;
		if (topb > 255) {
			topb = 255;
		}
		if (topb < 0) {
			topb = 0;
		}

		unsigned char bresult = (unsigned char)topb;

		top.set_blue(i, bresult);
	}
	return top;
}

Image addgreen(Image top, int num) {
	//cout << "adding to green..." << endl;

	vector <Pixel> topvec = top.get_pixels();
	for (int i = 0; i < length; ++i) {
		int topg = int((topvec[i]).green);

		topg += num;
		if (topg > 255) {
			topg = 255;
		}
		if (topg < 0) {
			topg = 0;
		}

		unsigned char gresult = (unsigned char)topg;

		top.set_green(i, gresult);
	}
	return top;
}

Image addred(Image top, int num) {
	//cout << "adding to red..." << endl;

	vector <Pixel> topvec = top.get_pixels();
	for (int i = 0; i < length; ++i) {
		int topr = int((topvec[i]).red);

		topr += num;
		if (topr > 255) {
			topr = 255;
		}
		if (topr < 0) {
			topr = 0;
		}

		unsigned char rresult = (unsigned char)topr;

		top.set_red(i, rresult);
	}
	return top;
}

Image scalered(Image top, int val) {
	//cout << "scaling red..." << endl;

	vector <Pixel> topvec = top.get_pixels();
	for (int i = 0; i < length; ++i) {
		int topr = int((topvec[i]).red);

		topr *= val;
		if (topr > 255) {
			topr = 255;
		}
		if (topr < 0) {
			topr = 0;
		}

		unsigned char rresult = (unsigned char)topr;

		top.set_red(i, rresult);
	}
	return top;
}

Image scaleblue(Image top, int val) {
	//cout << "scaling blue..." << endl;

	vector <Pixel> topvec = top.get_pixels();
	for (int i = 0; i < length; ++i) {
		int topb = int((topvec[i]).blue);

		topb *= val;
		if (topb > 255) {
			topb = 255;
		}
		if (topb < 0) {
			topb = 0;
		}

		unsigned char bresult = (unsigned char)topb;

		top.set_blue(i, bresult);
	}
	return top;
}

Image scalegreen(Image top, int val) {
	//cout << "scaling green..." << endl;

	vector <Pixel> topvec = top.get_pixels();
	for (int i = 0; i < length; ++i) {
		int topg = int((topvec[i]).green);

		topg *= val;
		if (topg > 255) {
			topg = 255;
		}
		if (topg < 0) {
			topg = 0;
		}

		unsigned char gresult = (unsigned char)topg;

		top.set_green(i, gresult);
	}
	return top;
}

Image onlyblue(Image top) {
	//cout << "reading blue chanel..." << endl;

	vector <Pixel> topvec = top.get_pixels();

	for (int i = 0; i < length; ++i) {
		unsigned char topb = (topvec[i]).blue;

		top.set_pixel(i, topb, topb, topb);
	}
	return top;
}

Image onlygreen(Image top) {
	//cout << "reading green chanel..." << endl;

	vector <Pixel> topvec = top.get_pixels();

	for (int i = 0; i < length; ++i) {
		unsigned char topg = (topvec[i]).green;

		top.set_pixel(i, topg, topg, topg);
	}
	return top;
}

Image onlyred(Image top) {
	//cout << "reading red chanel..." << endl;

	vector <Pixel> topvec = top.get_pixels();

	for (int i = 0; i < length; ++i) {
		unsigned char topr = (topvec[i]).red;

		top.set_pixel(i, topr, topr, topr);
	}
	return top;
}


Image combine(Image _blue, Image _green, Image _red) {
	//cout << "getting chanels..." << endl;

	vector <Pixel> greenvec = _green.get_pixels();
	vector <Pixel> redvec = _red.get_pixels();

	for (int i = 0; i < length; ++i) {
		unsigned char topg = (greenvec[i]).green;
		unsigned char topr = (redvec[i]).red;

		_blue.set_green(i, topg);
		_blue.set_red(i, topr);
	}
	return _blue;
}

Image flip(Image top) {
	//cout << "rotating..." << endl;

	vector <Pixel> topvec = top.get_pixels();

	for (int i = 0; i < length; ++i) {
		unsigned char topb = (topvec[length - 1 - i]).blue;
		unsigned char topg = (topvec[length - 1 - i]).green;
		unsigned char topr = (topvec[length - 1 - i]).red;

		top.set_pixel(i, topb, topg, topr);
	}
	return top;
}

int main(int argc, const char** argv)
{
	vector<string> methods = { "combine", "screen", "multiply" , "subtract", "overlay", "addred", "addgreen", "addblue", "scalered", "scalegreen", "scaleblue", "onlyred", "onlygreen", "onlyblue", "flip" };
	vector<string> reqpic = { "screen", "multiply" , "subtract", "overlay" };
	vector<string> reqnum = { "addred", "addgreen", "addblue", "scalered", "scalegreen", "scaleblue" };
	vector<string> reqnone = { "onlyred", "onlygreen", "onlyblue", "flip" };

	if (argc == 1) {
		cout << "Project 2: Image Processing, Spring 2023" << endl;
		cout << endl << "Usage:" << endl;
		cout << "\t./project2.out [output] [firstImage] [method] [...]" << endl;
	}
	if (argc == 2) {
		string temp1(argv[1]);
		if (temp1 == "--help") {
			cout << "Project 2: Image Processing, Spring 2023" << endl;
			cout << endl << "Usage:" << endl;
			cout << "\t./project2.out [output] [firstImage] [method] [...]" << endl;
		}
		else {
			cout << "Invalid file name." << endl;
			return 0;
		}
	}
	if (argc > 2) {
		string outfile = "";
		string target = "";
		string method = "";
		string secimage = "";
		string thirdimage = "";
		string numstr = "";
		int    numb = 0;
		Image firstimage;
		int forloopstart = 0;

		string temp2(argv[1]);
		string file = temp2;
		if (file.size() >= 4 && file.substr(file.size() - 4) == ".tga") {
			outfile = file;
			//cout << "output file path: " << outfile << endl;
		}
		else {
			cout << "Invalid file name." << endl;
			return 0;
		}

		string temp3(argv[2]);
		//cout << temp3 << endl;
		file = temp3;
		target = file;
		if (target.size() >= 4 && target.substr(target.size() - 4) == ".tga") {

			//check if it exists:
			ifstream file1(target, ios_base::binary);
			if (file1.is_open()) {
				//cout << "target file: " << target << endl;
				file1.close();
			}
			else {
				cout << "File does not exist." << endl;
				return 0;
			}
		}
		else {
			cout << "Invalid file name." << endl;
			return 0;
		}

		if (argc == 3) {
			cout << "Invalid method name." << endl;
			return 0;
		}

		//at least 4 arguments:
		else {
			string temp4(argv[3]);
			method = temp4;
			auto it = find(reqpic.begin(), reqpic.end(), method);
			auto it2 = find(reqnum.begin(), reqnum.end(), method);
			auto it3 = find(reqnone.begin(), reqnone.end(), method);

//combine method:
			if (method == "combine") {           //takes 2 images
				if (argc >= 5) {
					string temp5(argv[4]);
					file = temp5;
					if (file.size() >= 4 && file.substr(file.size() - 4) == ".tga") {

						//check if it exists:
						fstream file1(file);
						if (file1.is_open()) {
							secimage = file;
							file1.close();
						}
						else {
							cout << "Invalid argument, file does not exist." << endl;
							return 0;
						}

						if (argc >= 6) {
							string temp6(argv[5]);
							file = temp6;
							if (file.size() >= 4 && file.substr(file.size() - 4) == ".tga") {

								//check if it exists:
								fstream file1(file);
								if (file1.is_open()) {
									thirdimage = file;
									file1.close();
								}
								else {
									cout << "Invalid argument, file does not exist." << endl;
									return 0;
								}
							}
							else {
								cout << "Invalid argument, invalid file name." << endl;
								return 0;
							}
						}
						else {
							cout << "Missing argument." << endl;
							return 0;
						}

					}
					else {
						cout << "Invalid argument, invalid file name." << endl;
						return 0;
					}
				}
				else {
					cout << "Missing argument." << endl;
					return 0;
				}

				//implement 1st method with 2 image inputs:
				firstimage = combine(read(thirdimage), read(secimage), read(target));

				forloopstart = 6;
			}
//methods with 1 image:
			else if (it != reqpic.end()) {      //takes 1 image				
				if (argc >= 5) {
					string temp7(argv[4]);
					file = temp7;
					if (file.size() >= 4 && file.substr(file.size() - 4) == ".tga") {

						//check if it exists:
						fstream file1(file);
						if (file1.is_open()) {
							secimage = file;
							file1.close();
						}
						else {
							cout << "Invalid argument, file does not exist." << endl;
							return 0;
						}


					}
					else {
						cout << "Invalid argument, invalid file name." << endl;
						return 0;
					}
				}
				else {
					cout << "Missing argument." << endl;
					return 0;
				}

				//implement 1st method with 1 image input:
				if (method == "screen") {
					firstimage = screen(read(secimage), read(target));
				}
				else if (method == "multiply") {
					firstimage = multiply(read(secimage), read(target));
				}
				else if (method == "subtract") {
					firstimage = subtract(read(target), read(secimage));
				}
				else if (method == "overlay") {
					firstimage = overlay(read(target), read(secimage));
				}

				forloopstart = 5;
			}
//methods with 1 numbet:
			else if (it2 != reqnum.end()) {     //takes 1 number
				if (argc >= 5) {
					string temp8(argv[4]);
					numstr = temp8;
					try {
						numb = stoi(numstr);
					}
					catch (invalid_argument&) {
						cout << "Invalid argument, expected number." << endl;
						return 0;
					}

				}
				else {
					cout << "Missing argument." << endl;
					return 0;
				}

				//implement 1st method with 1 number input:
				if (method == "addred") {
					firstimage = addred(read(target), numb);
				}
				if (method == "addgreen") {
					firstimage = addgreen(read(target), numb);
				}
				if (method == "addblue") {
					firstimage = addblue(read(target), numb);
				}
				if (method == "scalered") {
					firstimage = scalered(read(target), numb);
				}
				if (method == "scalegreen") {
					firstimage = scalegreen(read(target), numb);
				}
				if (method == "scaleblue") {
					firstimage = scaleblue(read(target), numb);
				}

				forloopstart = 5;
			}
//methods with mo input:
			else if (it3 != reqnone.end()) {    //no arguments

				//implement 1st method with NO inputs:
				if (method == "onlyred") {
					firstimage = onlyred(read(target));
				}
				if (method == "onlygreen") {
					firstimage = onlygreen(read(target));
				}
				if (method == "onlyblue") {
					firstimage = onlyblue(read(target));
				}
				if (method == "flip") {
					firstimage = flip(read(target));
				}

				forloopstart = 4;
			}
			else {
				cout << "Invalid method name." << endl;
				return 0;
			}

			//for loop for any other methods included:
			int i = forloopstart;
			int first_i = i;
			for (i; i < argc; ++i) {
				string temp9(argv[i]);
				method = temp9;

				auto it4 = find(methods.begin(), methods.end(), method);
				if (it4 != methods.end()) {

					auto it = find(reqpic.begin(), reqpic.end(), method);
					auto it2 = find(reqnum.begin(), reqnum.end(), method);
					auto it3 = find(reqnone.begin(), reqnone.end(), method);

//combine method:
					if (method == "combine") {           //takes 2 images
						if (argc >= i + 2) {
							string temp10(argv[i + 1]);
							file = temp10;
							if (file.size() >= 4 && file.substr(file.size() - 4) == ".tga") {

								//check if it exists:
								fstream file1(file);
								if (file1.is_open()) {
									secimage = file;
									file1.close();
								}
								else {
									cout << "Invalid argument, file does not exist." << endl;
									return 0;
								}

								if (argc >= i + 3) {
									string temp11(argv[i + 2]);
									file = temp11;
									if (file.size() >= 4 && file.substr(file.size() - 4) == ".tga") {

										//check if it exists:
										fstream file1(file);
										if (file1.is_open()) {
											thirdimage = file;
											file1.close();
										}
										else {
											cout << "Invalid argument, file does not exist." << endl;
											return 0;
										}
									}
									else {
										cout << "Invalid argument, invalid file name." << endl;
										return 0;
									}
								}
								else {
									cout << "Missing argument." << endl;
									return 0;
								}

							}
							else {
								cout << "Invalid argument, invalid file name." << endl;
								return 0;
							}
						}
						else {
							cout << "Missing argument." << endl;
							return 0;
						}

						//implement method with 2 image inputs:
						firstimage = combine(read(thirdimage), read(secimage), firstimage);

						if (argc > i + 3) {
							auto it5 = find(methods.begin(), methods.end(), argv[i + 3]);
							if (it5 == methods.end()) {
								cout << "Invalid method name.";
								return 0;
							}

						}
					}
//methods with 1 image:
					else if (it != reqpic.end()) {      //takes 1 image				
						if (argc >= i + 2) {
							string temp12(argv[i + 1]);
							file = temp12;
							if (file.size() >= 4 && file.substr(file.size() - 4) == ".tga") {

								//check if it exists:
								fstream file1(file);
								if (file1.is_open()) {
									secimage = file;
									file1.close();
								}
								else {
									cout << "Invalid argument, file does not exist." << endl;
									return 0;
								}


							}
							else {
								cout << "Invalid argument, invalid file name." << endl;
								return 0;
							}
						}
						else {
							cout << "Missing argument." << endl;
							return 0;
						}

						//implement method with 1 image input:
						if (method == "screen") {
							firstimage = screen(read(secimage), firstimage);
						}
						else if (method == "multiply") {
							firstimage = multiply(firstimage, read(secimage));
						}
						else if (method == "subtract") {
							firstimage = subtract(firstimage, read(secimage));
						}
						else if (method == "overlay") {
							firstimage = overlay(firstimage, read(secimage));
						}
						if (argc > i + 2) {
							auto it5 = find(methods.begin(), methods.end(), argv[i + 2]);
							if (it5 == methods.end()) {
								cout << "Invalid method name.";
								return 0;
							}

						}
					}
//methods with 1 number:					
					else if (it2 != reqnum.end()) {     //takes 1 number
						if (argc >= i + 2) {
							string temp13(argv[i + 1]);
							numstr = temp13;
							try {
								numb = stoi(numstr);
							}
							catch (invalid_argument&) {
								cout << "Invalid argument, expected number." << endl;
								return 0;
							}

						}
						else {
							cout << "Missing argument." << endl;
							return 0;
						}

						//implement method with 1 number input:
						if (method == "addred") {
							firstimage = addred(firstimage, numb);
						}
						if (method == "addgreen") {
							firstimage = addgreen(firstimage, numb);
						}
						if (method == "addblue") {
							firstimage = addblue(firstimage, numb);
						}
						if (method == "scalered") {
							firstimage = scalered(firstimage, numb);
						}
						if (method == "scalegreen") {
							firstimage = scalegreen(firstimage, numb);
						}
						if (method == "scaleblue") {
							firstimage = scaleblue(firstimage, numb);
						}
						if (argc > i + 2) {
							auto it5 = find(methods.begin(), methods.end(), argv[i + 2]);
							if (it5 == methods.end()) {
								cout << "Invalid method name.";
								return 0;
							}

						}

					}
//methods with no inputs:
					else if (it3 != reqnone.end()) {    //no arguments

						//implement method with NO inputs:
						if (method == "onlyred") {
							firstimage = onlyred(firstimage);
						}
						if (method == "onlygreen") {
							firstimage = onlygreen(firstimage);
						}
						if (method == "onlyblue") {
							firstimage = onlyblue(firstimage);
						}
						if (method == "flip") {
							firstimage = flip(firstimage);
						}

						if (argc > i+1) {
							auto it5 = find(methods.begin(), methods.end(), argv[i+1]);
							if (it5 == methods.end()) {
								cout << "Invalid method name.";
								return 0;
							}
							
						}
					}
					
				}
				else {
					if (i == first_i) {
						cout << "Invalid method name.";
						return 0;
					}
				}
			}
			write(outfile, firstimage);
		}
	}


	return 0;

	//example commands without using command line:
	
	//task 1:	
	write("output/part1.tga", multiply(read("input/layer1.tga"), read("input/pattern1.tga")));

	//task 2:
	write("output/part2.tga", subtract(read("input/car.tga"), read("input/layer2.tga")));

	//task 3:
	write("output/part3.tga", screen(read("input/text.tga"), multiply(read("input/layer1.tga"), read("input/pattern2.tga"))));

	//task 4:
	write("output/part4.tga", subtract(multiply(read("input/layer2.tga"), read("input/circles.tga")), read("input/pattern2.tga")));

	//task 5:
	write("output/part5.tga", overlay(read("input/layer1.tga"), read("input/pattern1.tga")));

	//task 6:
	write("output/part6.tga", addgreen(read("input/car.tga"), 200));

	//task 7:
	write("output/part7.tga", scaleblue(scalered(read("input/car.tga"), 4), 0));

	//task 8:
	write("output/part8_b.tga", onlyblue(read("input/car.tga")));
	write("output/part8_g.tga", onlygreen(read("input/car.tga")));
	write("output/part8_r.tga", onlyred(read("input/car.tga")));

	//task 9:
	write("output/part9.tga", combine(read("input/layer_blue.tga"), read("input/layer_green.tga"), read("input/layer_red.tga")));

	//task 10:
	write("output/part10.tga", flip(read("input/text2.tga")));

	return 0;
}