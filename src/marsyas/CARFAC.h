/*
** Copyright (C) 1998-2011 George Tzanetakis <gtzan@cs.uvic.ca>
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

#ifndef MARSYAS_CARFAC_H
#define MARSYAS_CARFAC_H

#include "MarSystem.h"

#include "pngwriter.h"


namespace Marsyas
{
/**
   \ingroup Processing Basic Certified
   \brief Multiply input realvec with a fixed value.

   Multiply all the values of the input realvec with
   mrs_real/gain and put them in the output vector.

   Controls:
   - \b mrs_real/gain [w] : adjust the gain multiplier.

*/

class filter_state_class {
 public:
  std::vector<double> z1_memory;
  std::vector<double> z2_memory;
  std::vector<double> zA_memory;
  std::vector<double> zB_memory;
  std::vector<double> dzB_memory;
  std::vector<double> zY_memory;
  std::vector<double> detect_accum;

 public:
  filter_state_class();
  // filter_state_class(const filter_state_class& a);
  ~filter_state_class();

  friend std::ostream& operator<<(std::ostream&, const filter_state_class&);
  friend std::ostream& operator<<(std::ostream&, std::vector<std::vector<double> > a);
  friend std::ostream& operator<<(std::ostream&, std::vector<double> a);
};

class AGC_state_class {
 public:
  std::vector<std::vector<double> > AGC_memory;
  std::vector<double> AGC_sum;
  std::vector<double> sum_AGC;


 public:
  AGC_state_class();
  // AGC_state_class(const AGC_state_class& a);
  ~AGC_state_class();

  friend std::ostream& operator<<(std::ostream&, const AGC_state_class&);
  friend std::ostream& operator<<(std::ostream&, std::vector<double> a);
};

class filter_coeffs_class {
 public:
  double velocity_scale;

  std::vector<double> r_coeffs;
  std::vector<double> a_coeffs;
  std::vector<double> c_coeffs;
  std::vector<double> h_coeffs;
  std::vector<double> g_coeffs;

 public:
  filter_coeffs_class();
  // filter_coeffs_class(const filter_coeffs_class& a);
  ~filter_coeffs_class();

  void init(double velocity_scale, int n_ch);

  friend std::ostream& operator<<(std::ostream&, const filter_coeffs_class&);
};

class CF_AGC_params_class {
 public:
  int n_stages;
  std::vector<double> time_constants;
  int AGC_stage_gain;              // gain from each stage to next slower stage
  int decimation;                  // how often to update the AGC states
  std::vector<double> AGC1_scales; // in units of channels
  std::vector<double> AGC2_scales; // spread more toward base
  double detect_scale;             // max nap will be 10 to 100 typically
  double AGC_mix_coeff;

 public:
  CF_AGC_params_class();
  CF_AGC_params_class(const CF_AGC_params_class& a);
  ~CF_AGC_params_class();

  friend std::ostream& operator<<(std::ostream&, const CF_AGC_params_class&);
};

class AGC_coeffs_class {
 public:
  double detect_scale;
  double AGC_stage_gain;
  double AGC_mix_coeff;
  std::vector<double> AGC_epsilon;
  std::vector<double> AGC1_polez;
  std::vector<double> AGC2_polez;

 public:
  AGC_coeffs_class();
  // AGC_coeffs_class(const AGC_coeffs_class& a);
  ~AGC_coeffs_class();

  AGC_coeffs_class& operator=(const CF_AGC_params_class& a);
};


class CF_filter_params_class {
 public:
  double velocity_scale;
  double min_zeta;
  double first_pole_theta;
  double zero_ratio;
  double ERB_per_step;
  double min_pole_Hz;

 public:
  CF_filter_params_class();
  // CF_filter_params_class(const CF_filter_params_class& a);
  ~CF_filter_params_class();

  friend std::ostream& operator<<(std::ostream&, const CF_filter_params_class&);
};

class CF_class {
 public:
  int fs; // Sampling frequency
  CF_filter_params_class CF_filter_params;
  CF_AGC_params_class CF_AGC_params;
  int n_ch; // Number of channels
  std::vector<double> pole_freqs;
  filter_coeffs_class filter_coeffs;
  AGC_coeffs_class AGC_coeffs;
  int n_mics; // Number of microphones (input observations)
  std::vector<filter_state_class> filter_state;
  std::vector<AGC_state_class> AGC_state;
  std::vector<std::vector<std::vector<double> > > nap;
  int cum_k; // Global time step in concatenated segments

 public:
  CF_class();
  CF_class(const CF_class& a);
  ~CF_class();

  void CARFAC_Design(double fs = -1, double ERB_break_freq = -1, double ERB_q = -1);
  void CARFAC_DesignFilters(CF_filter_params_class params, double fs, std::vector<double> pole_freqs);
  void CARFAC_DesignAGC(double fs);
  void CARFAC_Init(int n_mics = -1);

  double ERB_Hz(double CF_Hz, double ERB_break_freq = -1, double ERB_Q = -1);

  friend std::ostream& operator<<(std::ostream&, const CF_class&);
};


class CARFAC: public MarSystem
{

 private:
  //Add specific controls needed by this MarSystem.
  void addControls();
  void myUpdate(MarControlPtr sender);

  MarControlPtr ctrl_gain_;
  mrs_real gainValue_; // for efficency

  CF_class CF;

  std::vector<double> CARFAC_FilterStep(std::vector<double> input_waves, int mic);

  void CARFAC_AGCStep(std::vector<std::vector<double> > avg_detects);
  std::vector<double> filter(std::vector<double> a, std::vector<double>b, std::vector<double> x, std::vector<double>& state);

 public:
  CARFAC(std::string name);
  CARFAC(const CARFAC& a);
  ~CARFAC();
  MarSystem* clone() const;


  std::string toString();
  void printParams();
  void myProcess(realvec& in, realvec& out);
};

}//namespace Marsyas

#endif