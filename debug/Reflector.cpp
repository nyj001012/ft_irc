/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reflector.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heshin <heshin@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 09:58:17 by heshin            #+#    #+#             */
/*   Updated: 2023/11/16 01:24:12 by heshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reflector.hpp"
#include "Serializable.hpp"
#include "../include/debug.hpp"
#include <ctime>
#include <exception>
#include <iostream>
#include <sstream>
#include <utility>
#define OUTPUT_FILE "data.json"

using std::ofstream;
using std::string;
using std::vector;
using std::cerr;
using std::stringstream;
using std::pair;
using std::make_pair;

Reflector::Reflector(): depth(3) {
	try {
		outfile.open(OUTPUT_FILE, std::ios::out | std::ios::trunc);
		outfile.close();
	} catch(std::exception &e) {
		cerr << e.what() << std::endl;
	}
	if (!outfile.good()) {
		cerr << "Fail to open " << OUTPUT_FILE << std::endl;
	}
}

Reflector::~Reflector() {
}

void Reflector::add(const Serializable& target) {
	targets.push_back(make_pair(&target, target._serialize(2)));
	update(message);
}

void Reflector::set_depth(const int depth) {
	this->depth = depth;
	update(message);
}

void Reflector::update() {

	vector<pair<const Serializable*, string> >::iterator iter;
	for (iter = targets.begin(); iter != targets.end(); ++iter) {
		iter->second = iter->first->_serialize(depth);
	}
	outfile.open(OUTPUT_FILE, std::ios::out | std::ios::trunc);
	if (!outfile.good())
		return ;
	string content = generate();
	outfile << content;
	outfile.close();
}

void Reflector::update(const string& message) {
	this->message = message;	
	update();
}

string Reflector::generate() const {
	stringstream ss;	
	ss << '{';
	string time = get_time_stamp();
	if (!time.empty())
		_json(ss, "time", ':', time, ',');
	_json(ss, "message", ':', message, ',');
	vector<pair<const Serializable*, string> >::const_iterator iter;
	for (iter = targets.begin(); iter != targets.end(); ++iter) {
		_json(ss, iter->first->_get_label(), ':');
		ss << iter->second;
		if (iter + 1 != targets.end())
			ss << ',';
	}
	ss << '}';
	return ss.str();
}

Reflector& Reflector::shared() {
	static Reflector r;

	return r;
}

