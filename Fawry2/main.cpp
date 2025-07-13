#include<bits/stdc++.h>
class Book{
    private:std::string title;
    private:std::string ISBN;
    private:int publication_date;
    private:double price;
    private:int quantity;
    
    public:Book(std::string title,std::string ISBN,
    int publication_date, double price,int quantity){
        
        this->title=title;
        this->ISBN=ISBN;
        this->publication_date;
        this->price=price;
        this->quantity=quantity;
        
    }
    
    public:void setQuantity(int quantity){
        this->quantity=quantity;
    }
    
    public:std::string getTitle(){
        return this->title;
    }
    
    public:std::string getISBN(){
        return this->ISBN;
    }
    
    public:int getPublication_date(){
        return this->publication_date;
    }
    
    public:double getPrice(){
        return this->price;
    }
    
    public:int getQuantity(){
        return this->quantity;
    }
    
    virtual ~Book()=default;
};

class Paper_book:public Book{
    private:bool is_shippable;
    
    public:Paper_book(std::string title,std::string ISBN,
    int publication_date, double price,int quantity,
    bool is_shippable):Book(title,ISBN,publication_date,price,quantity){
        
        this->is_shippable=is_shippable;
        
    }
    
    
    public:bool Is_shippable(){
        return this->is_shippable;
    }
    
    
};

class EBook:public Book{
    private:std::string filetype;
    private:bool emailable;
    
    public:EBook(std::string title,std::string ISBN,
    int publication_date, double price,int quantity,std::string filetype,
    bool emailable):Book(title,ISBN,publication_date,price,quantity){
        
        this->filetype=filetype;
        this->emailable=emailable;
        
    }
    
    public:bool send_to_Email(){
        return this->emailable;
    }
    
};

class Showcase:public Book{
    
    public:Showcase(std::string title,std::string ISBN,
    int publication_date, double price,int quantity):Book(title,ISBN,publication_date,price,quantity){
    }
    
    public:bool Sales(){
        return false;
    }
    
};



class Inventory{
    private:std::vector<std::shared_ptr<Book>>books;
    
    public:void add_books(std::shared_ptr<Book>book){
        books.push_back(book);
    }
    
    public:std::vector<std::shared_ptr<Book>> Remove_Return(double years){
        std::vector<std::shared_ptr<Book>>kicked_books;
        for(auto it=books.begin();it!=books.end();){
            if((2025-(*it)->getPublication_date())>years){
                kicked_books.push_back(*it);
                it=books.erase(it);
            }
            else{
                it++;
            }
        }
        return kicked_books;
    }
    
    public:std::shared_ptr<Book>searchByISBN(std::string ISBN){
        for(const auto&book:books){
            if(book->getISBN()==ISBN){
                return book;
            }
        }
        return nullptr;
    }
    
    public:std::vector<std::shared_ptr<Book>> getBooks(){
        return books;
    }
    
    
};

class ShippingService{
    
    public:void get(std::shared_ptr<Book>copy,std::string Address){
        // Implmentation here
    }
    
};

class MailService{
    
    public:void get(std::shared_ptr<Book>copy,std::string email){
        // Implmentation here
    }
    
};


class CheckOutService{
    
    private:Inventory&inventory;
    private:ShippingService shipping;
    private:MailService mailService;
    
    public:CheckOutService(Inventory&inv):inventory(inv){};
    
    public:double BuyBook(std::string ISBN,int qty,std::string email,std::string address){
        std::shared_ptr<Book>book=inventory.searchByISBN(ISBN);
        
        int quantity=book->getQuantity();
        // Paper-book
        if(quantity>=0){
            
            if(quantity>=qty){
                book->setQuantity(quantity-qty);
            }
            
            else{
                throw std::runtime_error("Insufficient stock");
            }
            
            if(book->Is_shippable()){
                shipping.get(book,address);
                return book->getPrice()*qty;
            }
            
            else{
               throw std::runtime_error("this item cann't be shipped"); 
            }
        }
        
        else if(quantity<0){
            // Ebook
            if(quantity==-1){
                if(book->send_to_Email()){
                    mailService.get(book,email);
                    return book->getPrice();
                }
                else{
                   throw std::runtime_error("this item cann't be shipped"); 
                }
            }
        }
        return 0.0;
    }
    
};


class BookStoreTesting{
    private:CheckOutService placingOrder;
    private:Inventory inventoryTeaster;

    
    public:void add_Test(){
        std::shared_ptr<Book>b1=std::make_shared<Paper_book>("Title1", "ISBN1", 2023, 29.99, 3,true);
        std::shared_ptr<Book>b2=std::make_shared<EBook>("Title2", "ISBN2", 2023, 21, -1,"PDF",true);
        std::shared_ptr<Book>b3=std::make_shared<EBook>("Title3", "ISBN3", 2024, 15, -1,"PDF",false);
        
        inventoryTeaster.add_books(b1);
        inventoryTeaster.add_books(b2);
        inventoryTeaster.add_books(b3);
        
        for(auto book:inventoryTeaster.getBooks()){
            std::cout<<book->getTitle()<<" "<<book->getPrice()<<"\n";
        }
        
    }
    public:void Remove_Test(int year){
        inventoryTeaster.Remove_Return(year);
    }
    public:void Buy_Test(std::string ISBN,int qty,std::string email,std::string address){
        double paid_amount=placingOrder.BuyBook(ISBN,qty,email,address);
    }
};

int main(int argc,const char*argv[]){
    return(0);
}