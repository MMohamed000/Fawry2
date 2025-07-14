#include<bits/stdc++.h>
class Book{
    protected:std::string title;
    protected:std::string ISBN;
    protected:int publication_date;
    protected:double price;
    
    public:Book(std::string title,std::string ISBN,
    int publication_date, double price){
        
        this->title=title;
        this->ISBN=ISBN;
        this->publication_date=publication_date;
        this->price=price;
        
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
    
    virtual ~Book()=default;
};

class Paper_book:public Book{
    private:int Stock;
    private:bool is_shippable;
    
    public:Paper_book(std::string title,std::string ISBN,
    int publication_date, double price,int Stock,
    bool is_shippable):Book(title,ISBN,publication_date,price){
        
        this->Stock=Stock;
        this->is_shippable=is_shippable;
        
    }
    
    public:void setStock(int quantity){
        this->Stock=quantity;
    }
    
    public:int getStock(){
        return this->Stock;
    }
    
    public:bool Is_shippable(){
        return this->is_shippable;
    }
    
};

class EBook:public Book{
    private:std::string filetype;
    private:bool emailable;
    
    public:EBook(std::string title,std::string ISBN,
    int publication_date, double price,std::string filetype,
    bool emailable):Book(title,ISBN,publication_date,price){
        
        this->filetype=filetype;
        this->emailable=emailable;
        
    }
    
    public:bool send_to_Email(){
        return this->emailable;
    }
};

class Showcase:public Book{
    
    public:Showcase(std::string title,std::string ISBN,
    int publication_date, double price):Book(title,ISBN,publication_date,price){
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
    
    public:void get(std::shared_ptr<Paper_book>copy,std::string Address){
        // Implmentation here
    }
    
};

class MailService{
    
    public:void get(std::shared_ptr<EBook>copy,std::string email){
        // Implmentation here
    }
    
};



enum class BookType{
    PaperBook,
    EBook,
    DemoBook
};

class CheckOutService{
    
    private:Inventory&inventory;
    private:ShippingService shipping;
    private:MailService mailService;
    
    public:CheckOutService(Inventory&inv):inventory(inv){};
    
    public:double BuyBook(std::string ISBN,int qty,std::string email,std::string address,BookType type){
        std::shared_ptr<Book>book=inventory.searchByISBN(ISBN);
        
        if(!book)throw std::runtime_error("Book Got Deleted.");
        
        double amount=0.0;
        
        switch(type){
            
            case BookType::PaperBook:{
                std::shared_ptr<Paper_book>copy=std::dynamic_pointer_cast<Paper_book>(book);
                
                if(copy->getStock()<qty){
                    throw std::runtime_error("No Stock.");
                }
                
                else{
                    if(copy->Is_shippable()){
                        copy->setStock(copy->getStock()-qty);
                        amount+=copy->getPrice()*qty;
                        // std::cout<<"Remaining Stock: "<<copy->getStock()<<"\n";
                        // Send to a shipping Service;
                        shipping.get(copy,address);
                    }else{
                        throw std::runtime_error("Book cannot being shipped.");
                    }
                }
                
                break;
            }
            
            case BookType::EBook:{
                std::shared_ptr<EBook>copy=std::dynamic_pointer_cast<EBook>(book);
                
                if(copy->send_to_Email()){
                    mailService.get(copy,email);
                    amount=copy->getPrice();
                }else{
                    throw std::runtime_error("Book cannot being sent via email.");
                }
                
                break;
            }
            
            case BookType::DemoBook: {
                throw std::runtime_error("Showcase books are not for sale.");
                break;
            }

        }
        
        return amount;
    }
    
};


class BookStoreTesting{
    private:Inventory inventoryTeaster;
    private:CheckOutService placingOrder;

    public:BookStoreTesting():placingOrder(inventoryTeaster){}
    
    public:void add_Test(){
        
        std::shared_ptr<Book>b1=std::make_shared<Paper_book>("Title1", "ISBN1", 2021, 29.99, 3,true);
        std::shared_ptr<Book>b0=std::make_shared<Paper_book>("Title4", "ISBN0", 2021, 29.99, 3,false);
        std::shared_ptr<Book>b2=std::make_shared<EBook>("Title2", "ISBN2", 2021, 21,"PDF",true);
        std::shared_ptr<Book>b3=std::make_shared<EBook>("Title3", "ISBN3", 2024, 15,"PDF",false);
        
        inventoryTeaster.add_books(b1);
        inventoryTeaster.add_books(b2);
        inventoryTeaster.add_books(b3);
        inventoryTeaster.add_books(b0);
        
        for(auto book:inventoryTeaster.getBooks()){
            std::cout<<"Book Title: "<<book->getTitle()<<" "<<"Book Price: "<<book->getPrice()<<"$\n";
        }

    }
    public:void Remove_Test(int year){
        
        auto books=inventoryTeaster.Remove_Return(year);
        
        for(auto book:books){
            std::cout<<"Book Removed:"<<book->getTitle()<<"\n";
        }
        
    }
    
    public:void Buy_Test(std::string ISBN,int qty,std::string email,std::string address,BookType type){
        
        double paid_amount=0.0;
        
        paid_amount=placingOrder.BuyBook(ISBN,qty,email,address,type);
        
        std::cout<<"Paid Amount:"<<paid_amount<<"$"<<std::endl;
    }
};

int main(int argc,const char*argv[]){
    // BookStoreTesting testing;
    // testing.add_Test();
    // // testing.Remove_Test(2);
    // testing.Buy_Test("ISBN1",2,"mohamed.saad@gmail.com","Fawry Bulding A",BookType::PaperBook);
    return(0);
}
