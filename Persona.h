#include<iostream>
using namespace std;

class Persona{
    private:
     int DNI[8];
     string nombre;
     string apellido;
     string lugar_nacimiento;
     string direccion;       
     int telefono;
     string correo_electronico;
     string estado_civil;
    public:
        Persona(int DNI[8], string nombre, string apellido, string lugar_nacimiento, string direccion, int telefono, string correo_electronico, string estado_civil){
            this->DNI[8]=DNI[8];
            this->nombre=nombre;
            this->apellido=apellido;
            this->lugar_nacimiento=lugar_nacimiento;
            this->direccion=direccion;       
            this->telefono=telefono;
            this->correo_electronico=correo_electronico;
            this->estado_civil=estado_civil;
        }

        //Setters
        void setDNI(int DNI[8]){    
            this->DNI[8]=DNI[8];
        }
        void setNombre(string nombre){
            this->nombre=nombre;
        }
        void setApellido(string apellido){
            this->apellido=apellido;
        }
        void setLugarNacimiento(string lugar_nacimiento){    
            this->lugar_nacimiento=lugar_nacimiento;
        }
        void setDireccion(string direccion){
            this->direccion=direccion;       
        }
        void setTelefono(int telefono){    
            this->telefono=telefono;
        }
        void setCorreoElectronico(string correo_electronico){    
            this->correo_electronico=correo_electronico;
        }
        void setEstadoCivil(string estado_civil){    
            this->estado_civil=estado_civil;
        }
        //Getters
        int getDNI(){    
            return DNI[8];
        }
        string getNombre(){
            return nombre;
        }
        string getApellido(){
            return apellido;
        }
        string getLugarNacimiento(){    
            return lugar_nacimiento;
        }
        string getDireccion(){
            return direccion;       
        }
        int getTelefono(){    
            return telefono;
        }
        string getCorreoElectronico(){    
            return correo_electronico;
        }
        string getEstadoCivil(){    
            return estado_civil;
        }
};