#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#include <iomanip>

using namespace std;

//get user data into basket
void user_basket(map<string, int> &basket, map<string, float>& products)
{
	//get input from users until they press X
	while (true)
	{
		cout << "Please insert a valid product or insert X to exit" << endl;
		string user_input;
		int quantity = 0;

		getline(cin, user_input);

		//check if the product entered is valid
		if (user_input == "X")
		{
			break;
		}
		else if (products.find(user_input) == products.end())
		{
			cout << "Invalid product, please try again" << endl;
		}
		else
		{
				cout << "Insert valid quantity" << endl;

				while (true)
				{
					//get quantity
					cin >> quantity;
					//if the quantity is a valid number and not 0
					if (cin.good())
					{
						if (quantity > 0)
						{
							basket[user_input] = quantity;
							//clear cin
							cin.ignore();
							break;
						}
					}
				cout << "Invalid quantity, please try again" << endl;
			}
		}
	}
}

//apply discounts in basket
void apply_discounts(map<string, int>& basket, map<string, float>& products, map<string, float>& discount)
{
	map<string, float>::iterator it_products;

	for (std::map<string, int>::iterator it_basket = basket.begin(); it_basket != basket.end(); ++it_basket)
	{
		//discount quantities
		int disc_quant = 0;
		//if the customer has two or more butters
		if (it_basket->first == "Butter" && it_basket->second >= 2)
		{
			//find if the customer has bread in their basket
			it_products = products.find("Bread");
			if (it_products != products.end())
			{
				//increment for every 4 
				disc_quant = (it_basket->second) / 2;
				discount[it_products->first] = it_products->second * (float)-0.5 * disc_quant;
			}
		}
		//if the customer has four or more milks
		else if (it_basket->first == "Milk" && it_basket->second >= 4)
		{
			//increment for every 4 
			disc_quant = (it_basket->second) / 4;
			//find milk in products and add minus price for every 4 milks in discount
			it_products = products.find("Milk");
			//might need else ************
			if (it_products != products.end())
				discount[it_products->first] = it_products->second * -1 * disc_quant;
		}
		else
		{
			cout << "Product could not be found in the database" << endl;
		}
	}
}

//print final receipt
void print_total(map<string, int>& basket, map<string, float>& products, map<string, float>& discount)
{
	map<string, float>::iterator it_products;
	float final_price = 0, discount_price = 0;

	cout << "The final order is:" << endl;
	//find values of basket products from products list and print basket
	for (std::map<string, int>::iterator it_basket = basket.begin(); it_basket != basket.end(); ++it_basket)
	{
		float product_price = 0.00;
		int product_quantity = it_basket->second;

		it_products = products.find(it_basket->first);
		if (it_products != products.end())
		{
			product_price = it_products->second;
		}
		product_price = product_price * product_quantity;
		cout << it_basket->first << " x " << product_quantity << " " << setprecision(2) << product_price << " GBP" << endl;
		final_price += product_price;
	}
	//print total price
	cout << "Total Price:" << " " << fixed << setprecision(2) << final_price << " GBP" << endl;

	//print discounts, if any
	cout << endl;
	cout << "Discounts:" << endl;
	for (std::map<string, float>::iterator it_disc = discount.begin(); it_disc != discount.end(); ++it_disc)
	{
		cout << it_disc->first << " " << it_disc->second << " GBP"<< endl;
		discount_price += -1 * it_disc->second;
		final_price += it_disc->second;
	}
	//print total discount
	cout << "Total savings:" << fixed << setprecision(2) << discount_price << " GBP" << endl;

	//print final price
	cout << "Final Price:" << " " << fixed << setprecision(2) << final_price << " GBP" <<  endl;
}

//get products from text file "Products.txt"
//separated product with semicolon in case of more than one word product
void get_products(map<string, float>& products)
{
	//create a stream to read
	ifstream source;
	stringstream s;
	//open file
	source.open("Products.txt", ios_base::in);
	//read each line from stream
	for (string line; getline(source, line); )
	{
		//make the line a stream
		istringstream ln_strm(line);

		string product;
		float price;
		//semicolon separated products
		while (std::getline(ln_strm, product, ';')) {

			ln_strm >> price;
			//if price is invalid stop the program
			if (price < 0)
			{
				cout << "Invalid price in text file please check and try again." << endl;
				exit(0);
			}
		}
		//add product and price to list
		products[product] = price;
	}
}

int main()
{
	map<string, int> basket;
	map<string, float> products;
	map<string, float> discount;

	get_products(products);

	user_basket(basket, products);
	apply_discounts(basket, products, discount);
	print_total(basket, products, discount);
	return 0;
}



