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
    \class RealvecSource

*/

#include "RealvecSource.h"

using namespace std;
using namespace Marsyas;

RealvecSource::RealvecSource(string name):MarSystem("RealvecSource",name)
{
  //type_ = "RealvecSource";
  //name_ = name;
  
	count_= 0;

	addControls();
}

RealvecSource::RealvecSource(const RealvecSource& a):MarSystem(a)
{
ctrl_data_ = getctrl("mrs_realvec/data");
}


RealvecSource::~RealvecSource()
{
}


MarSystem* 
RealvecSource::clone() const 
{
  return new RealvecSource(*this);
}

void 
RealvecSource::addControls()
{
  samplesToUse_ = (mrs_natural)MRS_DEFAULT_SLICE_NSAMPLES;
  addctrl("mrs_natural/samplesToUse", (mrs_natural)MRS_DEFAULT_SLICE_NSAMPLES);
  setctrlState("mrs_natural/samplesToUse", true);
  addctrl("mrs_bool/done", false);
  setctrlState("mrs_bool/done", true);
  addctrl("mrs_realvec/data", realvec(), ctrl_data_);
  setctrlState("mrs_realvec/data", true);
	 setctrlState("mrs_real/israte", true);
}


void
RealvecSource::myUpdate(MarControlPtr sender)
{
  MRSDIAG("RealvecSource.cpp - RealvecSource:myUpdate");
  
		const realvec& data = ctrl_data_->to<realvec> ();

  setctrl("mrs_natural/onObservations", data.getRows());
	setctrl("mrs_natural/onSamples", 1);
  setctrl("mrs_real/osrate", getctrl("mrs_real/israte")->toReal());
//  samplesToUse_ = getctrl("mrs_natural/samplesToUse")->toNatural();
 
	samplesToUse_ = data.getCols();
 count_ = 0;

		if( getctrl("mrs_bool/done")->isTrue()){
    count_ = 0;
    setctrl("mrs_bool/done", false);
  }
}

void 
RealvecSource::myProcess(realvec& in, realvec& out)
{
	//checkFlow(in,out);
	const realvec& data = ctrl_data_->to<realvec> ();

	if( count_ < samplesToUse_)
	{
		for (o=0; o < onObservations_; o++)
			out(o) = data(o, count_);
			count_++;
		}
	else
		setctrl("mrs_bool/done", true);  

	//out.dump();
	}


