Form::GradeTooHighException::GradeTooHighException(void) {
	this->_message = "The form's grade is too high";
	return ;
}

Form::GradeTooHighException::GradeTooHighException(std::string const message) : _message(message) {
	return ;
}	

Form::GradeTooHighException::GradeTooHighException(GradeTooHighException const& src) {
	*this = src;
	return ;
}

Form::GradeTooHighException::~GradeTooHighException(void) throw() {
	return ;
}

Form::GradeTooHighException&	Form::GradeTooHighException::operator=(GradeTooHighException const& other) {
	if (this != &other) {
		(void)other;
	}
	return (*this);
}

char const* Form::GradeTooHighException::what(void) const throw() {
	return (this->_message.c_str());
}