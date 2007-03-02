/*
** Copyright (C) 1998-2006 George Tzanetakis <gtzan@cs.uvic.ca>
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/


/** 
    \class Peaker
    \brief Pick peaks out of signal 

   Peaker is used to select peaks(or valleys) from the input vector. Various 
parameters of the peak selection process can be adjusted.
*/

#ifndef MARSYAS_PEAKER_H
#define MARSYAS_PEAKER_H

#include "MarSystem.h"	

namespace Marsyas
{

class Peaker: public MarSystem
{
private:
  mrs_real rms_;
  void addControls();

  mrs_natural twice_;
  mrs_natural half_;
  mrs_natural triple_;
  mrs_natural third_;
  
public:
  Peaker(std::string name);
  ~Peaker();
  MarSystem* clone() const;    

  void myProcess(realvec& in, realvec& out);
};

}//namespace Marsyas

#endif

	
