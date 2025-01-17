/*
 * shs_t.h
 *
 *  Created on: 14 Nov 2017
 *      Author: nathalie
 */

#include "SSModels.h"
#include <ginac/ginac.h>
#include <random>

#ifndef STOCHY_COMMON_SHS_H
#define STOCHY_COMMON_SHS_H

// General class template to cater for all the different SHS model definitions
template <class T, class T2> class shs_t {
public:
  T2 Q;  // Discrete modes
  T Tq;  // Discrete kernel- can represent either guards or probabilities
  int n; // Dimension of continuous state space in given mode
  arma::mat p_k;
  arma::mat q_0;                 // Modes
  std::vector<ssmodels_t> x_mod; // container of models
  exdata_t x_data;

private:
  bool shs; // 1- SHS, 0- HS
public:
  shs_t()
  {
    n = 1; // Dimension of continuous state space in given mode
    p_k = arma::mat(Q, n);
    q_0 = arma::mat(Q, 1); // Initial mode
    std::vector<ssmodels_t> x_mod;
    // Q = 1; // Discrete modes
    shs = 1;
    exdata_t x_data;
    std::vector<T> Tq;
  }
 
  shs_t(T2 disc, int num, T tq, std::vector<ssmodels_t> &old, exdata_t &data) 
  {
    Q = disc; // Discrete modes
    n = num;  // Dimension of continuous state space in given mode
    p_k = data.InitTq[0];
    q_0 = data.q_init[0]; // Initial mode
    x_mod = old;
    x_data = data;
    Tq = tq;
    shs = old[0].sigma.is_empty() ? 0 : 1;

    // Check dimensions of data
    int err = this->checkData();
    switch (err) 
    {
      case 3: 
      {
        std::cout << "Incorrect size of disturbance vector" << std::endl;
	break;
      }
      default: 
      {   
        std::cout << "Correct input vectors size" << std::endl;
	break;
      }
    }
  }

  shs_t(T2 disc, int num, T tq, std::vector<ssmodels_t> &old) 
  {
    Q = disc; // Discrete modes
    n = num;  // Dimension of continuous state space in given mode
    p_k = arma::zeros<arma::mat>(1, 1);
    q_0 = arma::zeros<arma::mat>(1, 1); // Initial mode

    p_k.empty();
    q_0.empty();

    x_mod = old;
    x_data = new exdata_t;
    Tq = tq;
    shs = old[0].sigma.is_empty() ? 0 : 1;
  }

  shs_t(T2 disc, int num, std::vector<ssmodels_t> &old, exdata_t &data) {
    Q = disc; // Discrete modes
    n = num;  // Dimension of continuous state space in given mode
    p_k = data.InitTq[0];
    q_0 = data.q_init[0]; // Initial mode
    x_mod = old;
    x_data = data;
    Tq = arma::zeros<arma::mat>(1, 1);
    shs = old[0].sigma.is_empty() ? 0 : 1;
    
    // Check dimensions of data
    int err = this->checkData();
    switch (err) 
    {
      case 2: 
      {
        std::cout << "Incorrect size of input vector" << std::endl;
	break;
       }
      case 3: 
      {
        std::cout << "Incorrect size of disturbance vector" << std::endl;
	break;
      }
      default:
      { 
        std::cout << "Correct input vectors size" << std::endl; 
        break;
      }
    }
  }

  shs_t(T tq, std::vector<ssmodels_t> &old, exdata_t &data) 
  {
    Q = 1;            // Discrete modes
    n = old[0].x_dim; // Dimension of continuous state space in given mode
    p_k = data.InitTq[0];
    q_0 = data.q_init[0]; // Initial mode
    x_mod = old;
    x_data = data;
    Tq = tq;
    shs = old[0].sigma.is_empty() ? 0 : 1;
    
    // Check dimensions of data
    int err = this->checkData();
    switch (err) 
    {
      case 3: 
      {
        std::cout << "Incorrect size of disturbance vector" << std::endl;
	break;
      }
      default: 
      {
	std::cout << "Correct input vectors size" << std::endl; 
	break;
      }
    }
  }
  virtual ~shs_t() {}

private:
  int checkData() 
  {
    int error = 0;
    if (this->x_mod[0].x_dim != this->x_data.X[0].n_rows) 
    {
      error = 1;
    }
    if (this->x_mod[0].u_dim != this->x_data.U.n_cols &&
        this->x_mod[0].u_dim > 0) 
    {
      error = 2;
    }
    if (this->x_mod[0].d_dim != this->x_data.D.n_cols &&
        this->x_mod[0].d_dim > 0) 
    {
      error = 3;
    }
    return error;
  }
};

/*************************************************************************************************/

// class template specialisation
// for the case when have a hybrid model and the transitions are governed by
// logical guards not probabilities
template <> class shs_t<std::vector<std::string>, int> {
public:
  int Q; // Discrete modes
  std::vector<std::string>
      Tq; // Discrete kernel- can represent either guards or probabilities
  int n;  // Dimension of continuous state space in given mode
  std::vector<arma::mat> p_k;
  arma::mat q_0;                 // Modes
  std::vector<ssmodels_t> x_mod; // container of models
  exdata_t x_data;

public:
  shs_t() {
    n = 1; // Dimension of continuous state space in given mode
    p_k = {arma::mat(Q, n)};
    q_0 = arma::mat(Q, 1); // Initial mode
    std::vector<ssmodels_t> x_mod;
    Q = 1; // Discrete modes
    exdata_t x_data;
    Tq = {" "};
  }
  shs_t(int disc, int num, std::vector<std::string> tq,
        std::vector<ssmodels_t> &old, exdata_t &data) {
    Q = disc; // Discrete modes
    n = num;  // Dimension of continuous state space in given mode
    p_k = {data.InitTq[0]};
    q_0 = data.q_init[0]; // Initial mode
    x_mod = old;
    x_data = data;
    Tq = tq;
    // Check dimensions of data
    int err = this->checkData();
    switch (err) {
    case 2: {
      std::cout << "Incorrect size of input vector" << std::endl;
    }
    case 3: {
      std::cout << "Incorrect size of disturbance vector" << std::endl;
    }
    default: { std::cout << "Correct input vectors size" << std::endl; }
    }
  }
  shs_t(std::vector<std::string> tq, std::vector<ssmodels_t> &old,
        exdata_t &data) {
    Q = 1;            // Discrete modes
    n = old[0].x_dim; // Dimension of continuous state space in given mode
    p_k = {data.InitTq[0]};
    q_0 = data.q_init[0]; // Initial mode
    x_mod = old;
    x_data = data;
    Tq = tq;
    // Check dimensions of data
    int err = this->checkData();
    switch (err) {
    case 2: {
      std::cout << "Incorrect size of input vector" << std::endl;
    }
    case 3: {
      std::cout << "Incorrect size of disturbance vector" << std::endl;
    }
    default: { std::cout << "Correct input vectors size" << std::endl; }
    }
  }
  shs_t(const char *fn, exdata_t &data) {
    // Obtain number of discrete modes and obtain Tq
    bool error = this->obtainTqfromMat(fn, *this);
    if (!error) {
      Q = this->Q;
      Tq = this->Tq;
      // Get transition probabilities associated with the guards
      // and store in pk
      p_k = {arma::mat(Q, Q)}; // obtainpk(*this);

      x_data = data;
      //  p_k = data.InitTq;
      q_0 = data.q_init[0]; // Initial mode
      // create array containing the ssmodels
      std::vector<ssmodels_t> models;
      arma::mat dummy = -1 * arma::ones<arma::mat>(1, 1);
      for (int i = 1; i <= Q; i++) {
        int x_dimension = data.x_k.n_rows;
        int u_dimension = data.u_k.n_rows;
        int d_dimension = data.d_k.n_rows;
        double lhs = arma::accu(data.u_k == dummy);
        double rhs = arma::accu(data.d_k == dummy);
        if ((lhs > 0) && (rhs > 0)) {
          u_dimension = 0;
          d_dimension = 0;
        } else if ((lhs > 0) && (rhs < 0)) {
          u_dimension = 0;
        }
        ssmodels_t mod(x_dimension, u_dimension, d_dimension);

        // Reading models
        std::cout
            << "Initialising model of continuous variables in discrete mode "
            << i << std::endl;
        mod.obtainSSfromMat(fn, mod, i);
        if ((unsigned)x_dimension != (unsigned)mod.x_dim ||
            (unsigned)x_dimension != (unsigned)mod.A.n_rows) {
          mod.obtainSSfromMat(fn, mod);
        }
        mod.checkModel(mod);
        models.push_back(mod);
      }
      x_mod = models;
      n = models[0].x_dim;
      // Check dimensions of data
      int err = this->checkData();
      switch (err) {
      case 3: {
        std::cout << "Incorrect size of disturbance vector" << std::endl;
      }
      default: { std::cout << "Correct input vectors size" << std::endl; }
      }
    }
  }
  shs_t(const char *fn) {
    // Obtain number of discrete modes and obtain Tq
    this->obtainTqfromMat(fn, *this);
    Q = this->Q;
    Tq = this->Tq;
    // Get transition probabilities associated with the guards
    // and store in pk
    p_k = {arma::mat(Q, Q)}; // obtainpk(*this);

    // create array containing the ssmodels
    std::vector<ssmodels_t> models;
    arma::mat dummy = -1 * arma::ones<arma::mat>(1, 1);
    for (int i = 1; i <= Q; i++) {
      int x_dimension = this->x_mod[0].A.n_rows;
      int u_dimension = this->x_mod[0].B.n_rows;
      int d_dimension = this->x_mod[0].F.n_rows;
      ssmodels_t mod(x_dimension, u_dimension, d_dimension);

      // Reading models
      std::cout
          << "Initialising model of continuous variables in discrete mode " << i
          << std::endl;
      mod.obtainSSfromMat(fn, mod, i);
      if ((unsigned)x_dimension != (unsigned)mod.x_dim ||
          (unsigned)x_dimension != (unsigned)mod.A.n_rows) {
        mod.obtainSSfromMat(fn, mod);
      }
      mod.checkModel(mod);
      models.push_back(mod);
    }
    x_mod = models;
    n = models[0].x_dim;
  }

  virtual ~shs_t() {}
  void step_hyb(shs_t &old, int n, int cond) {
    // TODO: Update to cater for multiple discrete modes
    int xdim = old.n;
    // arma::mat x_old=arma::mat(old.x_mod[(int) old.q_0(n,0)].x_dim,1);
    // x_old = old.x_data.X[n];
    arma::mat x_old(old.x_data.X[n]);
    arma::mat x_new = arma::mat(xdim, 1);

    double q_old = old.q_0(n, 0);
    double q_new = q_old;
    // std::vector<double> vec(x_old.size());
    // arma::mat x_old(vec);
    // Eigen::Map<arma::mat>(vec.data(), x_old.n_rows, x_old.n_cols) =x_old;
    // //TODO fix

    arma::mat u_old = arma::mat(old.x_mod[(int)old.q_0(n, 0)].u_dim, 1);
    u_old = old.x_data.u_k;
    arma::mat d_old = arma::mat(old.x_mod[(int)old.q_0(n, 0)].d_dim, 1);
    d_old = old.x_data.d_k;

    double curMode = q_old;
    std::cout << "Current mode: " << curMode << std::endl;
    std::cout << "Current temp: " << x_old << std::endl;

    if (old.Q > 1) {
      // Get discrete transition
      // Number of symbolic variables needed
      int kmax = old.NumSymbols(old);
      std::vector<std::string> x = old.Tq;
      // Generate list of symbols depending on
      // kmax, check type of guards whether simply a function of
      // x or is also of u and d
      GiNaC::lst syms = this->generateListofSymbols(x[kmax]);

      // For each transition from current mode, generate guard
      // Check if take guard or stay
      double index = curMode;
      double updated = 0;
      while (index < pow(old.Q, 2) && old.Q > 1) {
        std::cout << "Tq" << x[index] << std::endl;
        bool guard = this->getCurrentGuard(x[index], x_old, syms);
        if (guard && !updated) {
          // q_new = std::floor(index/old.Q);
          std::cout << "prob:" << old.p_k[0] << std::endl;
          std::cout << "q-old:" << q_old << std::endl;

          std::vector<int> q = t_q(old, q_old, 0);
          std::cout << "q" << q[0] << " " << q[1] << std::endl;
          q_new = q[1];
          std::cout << "q_new: " << q_new
                    << std::endl; //<< "index+1: "<< index+1
          updatepk(old, index);
          updated = 1;
        }
        index += old.Q;
      }
    }
    if (old.x_mod[(int)q_old].d_dim == 0) {
      x_new =
          old.x_mod[(int)q_new].updateLTI(old.x_mod[(int)q_old], x_old, u_old);
      // TODO Generalise for other models
      if (((q_old == 0 && q_new == 2) || (q_old == 1 && q_new == 0) ||
           (q_old == 2 && q_new == 0)) &&
          cond) {
        x_new(1, 0) = 0;
      }
    } else {
      x_new = old.x_mod[(int)q_new].updateLTIad(old.x_mod[(int)q_old], x_old,
                                                u_old, d_old);
    }
    old.x_data.x_k = x_new;
    // Append result to X in old object (columns)
    old.x_data.X.push_back(x_new);
    // Append result to Q in old object (columns)
    old.q_0(n + 1, 0) = q_new;

  }

  void step_hyb_ad(shs_t &old, int n, int cond, int steps) {
    arma::mat x_old(old.x_data.X[n]);
    arma::mat x_new = arma::mat(old.x_mod[(int)old.q_0(n, 0)].x_dim, steps);
    double q_old = old.q_0(n, 0);
    double q_new = q_old;
    arma::mat u_old = arma::mat(old.x_mod[(int)old.q_0(n, 0)].u_dim, 1);
    u_old = old.x_data.u_k;
    arma::mat d_old = arma::mat(old.x_mod[(int)old.q_0(n, 0)].d_dim, 1);
    d_old = old.x_data.d_k;

    if (old.Q == 1) {
      x_new = old.x_mod[0].updateLTIst(old.x_mod[0], x_old, u_old, d_old);
    } else {
      for (int j = 0; j < steps; j++) {
        // Get discrete transition
        // Number of symbolic variables needed
        int kmax = old.NumSymbols(old);
        std::vector<std::string> x = old.Tq;
        // Generate list of symbols depending on
        // kmax, check type of guards whether simply a function of
        // x or is also of u and d
        GiNaC::lst syms = this->generateListofSymbols(x[kmax]);

        // For each transition from current mode, generate guard
        // Check if take guard or stay
        double index = q_old;

        double updated = 0;
        while (index < pow(old.Q, 2) && old.Q > 1) {
          //  std::cout << "Tq" << x[index] <<std::endl;
          bool guard = this->getCurrentGuard(x[index], x_old, syms);
          if (guard && !updated) {
            std::vector<int> q = t_q(old, q_old, j);
            q_new = q[1];
            updatepk(old, index, j);
            updated = 1;
          }
          index += old.Q;
        }
        // Append result to Q in old object (columns)
        old.q_0(n + 1, j) = q_new;
        x_new.col(j) = old.x_mod[(int)q_new].updateLTIst(
            old.x_mod[(int)q_old], x_old.col(j), u_old, d_old);
      }
    }
    // Append result to X in old object (columns)
    old.x_data.X.push_back(x_new);


  }

  void run(shs_t &old, int N, int cond, int monte) {
    // Start simulation timers
    clock_t begin, end;
    begin = clock();
    double time = 0;
    // For deterministic case perform 1 run
    // For stochastic version perform Monte Carlo + compute mean
    int i = 0;
    while (i < N) {
      if (i == 0) {
        old.obtainpk(*this);
      }
      double a = old.x_mod[(int)old.q_0(i, 0)].sigma.n_rows;
      double b = old.x_mod[(int)old.q_0(i, 0)].sigma.n_cols;
      if ((a + b) > 3) {
        old.step_hyb_ad(old, i, cond, monte);
      } else {
        old.step_hyb(old, i, cond);
      }
      if ((unsigned)old.x_data.U.n_cols == (unsigned)N)
        old.x_data.u_k = old.x_data.U.row(i);
      if ((unsigned)old.x_data.D.n_cols == (unsigned)N)
        old.x_data.d_k = old.x_data.D.row(i);
      i++;
    }
    arma::mat y = old.x_data.X[0].t();

    end = clock();
    time = (double)(end - begin) / CLOCKS_PER_SEC;
    std::ostringstream oss;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    oss << std::put_time(&tm, "%d-%m-%Y-%H-%M-%S");
    auto str = oss.str();

    std::cout << std::endl;
    std::cout << "--------------------------------------" <<std::endl;
    std::cout << " Simulation time                      " << std::endl;
    std::cout << "--------------------------------------" <<std::endl;
    std::cout << " " << time << std::endl;
    std::cout << "--------------------------------------" <<std::endl;
    std::cout << std::endl;

    // Option to export to file results
    std::ofstream myfile;
    std::string exportOpt;
    std::string str0("y");
    std::string str1("yes");
    std::cout << "Would you like to store simulation results [y- yes, n - no] " << std::endl;
    std::cin >> exportOpt;
    if ((exportOpt.compare(str0) == 0) || (exportOpt.compare(str1) == 0)) {
      // check if results folder exists:
      if(checkFolderExists("../results") == -1) {
        if(mkdir("../results", 0777) == -1) {
          std::cerr << "Error cannot create results directory: " <<std::strerror(errno) <<std::endl;
          exit(0);
         }
      }
      // Store results in file
      std::string f_name = "../results/Simulationtime_" + str + ".txt";
      myfile.open(f_name);
      myfile << time <<std::endl;
      myfile.close();
      std::string y_name = "../results/y_" + str + ".txt";
      y.save(y_name, arma::raw_ascii);
      arma::mat q = old.q_0;
      std::string q_name = "../results/modes_" + str + ".txt";
      q.save(q_name, arma::raw_ascii);
      // TODO plotting
//      old.createPySimPlots(y, modes, x_dim);
    }

  }
  bool obtainTqfromMat(const char *fn, shs_t &init) {
    // Reading model file input in .arma::mat format
    // and storing into ssmodel class
    mat_t *matf;
    matvar_t *matvar, *contents;
    bool error = 0;
    // Read .arma::mat file
    matf = Mat_Open(fn, MAT_ACC_RDONLY);
    if (matf) // if successful in reading file
    {
      // read each variable within file and populate
      // state space model based on variable name
      contents = Mat_VarRead(matf, "Tq");
      if (contents == NULL) {
        std::cout << "Variable Tq not found in file" << std::endl;
        std::cout << "Number of modes set to 1" << std::endl;
        init.Q = 1;
      } else {
        init.populateTq(*contents);
        // Mat_VarFree(matvar);
        matvar = NULL;
        contents = NULL;
      }
      Mat_Close(matf);

    } else // unsuccessfull in opening file
    {
      return 1; // throw "Error opening mat file";
    }
  }
  void obtainpk(shs_t &init) {
    // Traverse Tq and get equialent probability matrix
    double index = 0;
    std::vector<std::string> str = init.Tq;

    arma::mat p = arma::mat(init.Q, init.Q);
    for (int j = 0; j < init.Q; j++) {
      for (int k = 0; k < init.Q; k++) {
        std::vector<std::string> spl = splitStr(str[index], ':');
        if (spl.size() == 1) {
          p(k, j) = 0;

        } else {
          p(k, j) = stod(spl[0]);
          init.Tq[index] = spl[1];
        }
        index += 1;
      }
    }
    p = checkStochasticity(p);
    std::cout << p << std::endl;
    for (unsigned int i = 0; i < init.p_k.size(); i++) {
      init.p_k[i] = p;
    }
  }
  void updatepk(shs_t &init, int currentGuardindex) {
    // Traverse Tq and get equialent probability matrix
    std::vector<std::string> str = init.Tq;
    arma::mat p = init.p_k[0];
    int row = currentGuardindex % init.Q;
    std::cout << "row : " << row << std::endl;
    int col = (int)currentGuardindex / init.Q;
    std::cout << "col: " << col << std::endl;
    int found1 = -1;
    for (unsigned j = 0; j < p.n_cols; j++) {
      if (p(row, j) == 1) {
        found1 = j;
      }
    }
    if (found1 != -1) {
      p(row, found1) = 0;
      p(row, col) = 1;
    }

    std::cout << "p: " << p << std::endl;
    p = checkStochasticity(p);
    std::cout << p << std::endl;

    init.p_k[0] = p;
  }
  void updatepk(shs_t &init, int currentGuardindex, int step) {
    // Traverse Tq and get equivalent probability matrix
    std::vector<std::string> str = init.Tq;
    arma::mat p = init.p_k[step];
    int row = currentGuardindex % init.Q;
    std::cout << "row : " << row << std::endl;
    int col = (int)(currentGuardindex) / init.Q;
    std::cout << "col: " << col << std::endl;
    std::cout << "p: " << p << std::endl;

    int found1 = -1;
    for (unsigned j = 0; j < p.n_cols; j++) {
      if (p(row, j) == 1) {
        found1 = j;
      }
    }
    if (found1 != -1) {
      p(row, found1) = 0;
      p(row, col) = 1;
    }

    std::cout << "p: " << p << std::endl;
    p = checkStochasticity(p);
    std::cout << p << std::endl;

    init.p_k[step] = p;
  }

private:
  GiNaC::symbol &get_symbol(const std::string &s) {
    static std::map<std::string, GiNaC::symbol> directory;
    std::map<std::string, GiNaC::symbol>::iterator i = directory.find(s);
    if (i != directory.end()) {
      return i->second;
    } else {
      return directory.insert(make_pair(s, GiNaC::symbol(s))).first->second;
    }
  }
  GiNaC::lst generateListofSymbols(std::string str) {
    GiNaC::lst symbols = {};

    // Check if there are 'x','u' or 'd' characters
    // If there are compute locations
    std::vector<int> x_symb = findLocation(str, 'x');
    std::vector<int> u_symb = findLocation(str, 'u');
    std::vector<int> d_symb = findLocation(str, 'd');
    // Convert strings to symbols as needed
    if (x_symb.size() > 0) {
      for (unsigned int i = 0; i < x_symb.size(); i++) {
        std::string str2 = str;
        if (str2.find('&') | str2.find('|')) {
          std::vector<std::string> x = splitStr(str2, '&');
          std::vector<std::string> y = splitStr(str2, '|');
          if (x.size() != str2.size()) {
            for (unsigned int j = 0; j < x.size(); j++) {
              //	   ;
              // 	   std::cout << x[j].substr(0,2)  << std::endl;
              GiNaC::symbol t = get_symbol(x[j].substr(0, 2));
              symbols.append(t);
            }
          }
          if (y.size() >= str.size() - 1) {
            for (unsigned int m = 0; m < y.size(); m++) {

              GiNaC::symbol t = get_symbol(y[m].substr(0, 2));
              symbols.append(t);
            }
          }
        } else {
          str2.substr(x_symb[i], 2);
          GiNaC::symbol t = get_symbol(str2);
          symbols.append(t);
        }
      }
    }
    if (u_symb.size() > 0) {
      int k = 0;
      for (unsigned int i = x_symb.size() - 1;
           i < x_symb.size() + u_symb.size() - 1; i++) {
        std::string str2 = str; //.substr(u_symb[k],str.size());
        if (str2.find('&') | str2.find('|')) {
          std::vector<std::string> x = splitStr(str2, '&');
          std::vector<std::string> y = splitStr(str2, '|');
          for (unsigned int j = 0; j < x.size(); j++) {

            GiNaC::symbol t = get_symbol(x[j].substr(0, 2));
            symbols.append(t);
          }
          for (unsigned int k = 0; k < y.size(); k++) {

            GiNaC::symbol t = get_symbol(y[k].substr(0, 2));
            symbols.append(t);
          }
        } else {
          str2.substr(u_symb[k], 2);
          GiNaC::symbol t = get_symbol(str2);
          symbols.append(t);
        }
        k++;
      }
    }

    if (d_symb.size() > 0) {
      int l = 0;
      for (unsigned int i = x_symb.size() + u_symb.size() - 1;
           i < x_symb.size() + u_symb.size() + d_symb.size() - 1; i++) {
        std::string str2 = str; //.substr(d_symb[l],str.size());
        if (str2.find('&') | str2.find('|')) {
          std::vector<std::string> x = splitStr(str2, '&');
          std::vector<std::string> y = splitStr(str2, '|');
          for (unsigned int j = 0; j < x.size(); j++) {

            GiNaC::symbol t = get_symbol(x[j].substr(0, 2));
            symbols.append(t);
          }
          for (unsigned int k = 0; k < y.size(); k++) {

            GiNaC::symbol t = get_symbol(y[k].substr(0, 2));
            symbols.append(t);
          }
        } else {
          str2.substr(d_symb[l], 2);
          GiNaC::symbol t = get_symbol(str2);
          symbols.append(t);
        }
        l++;
      }
    }
    return symbols;
  }
  // Function to determine the type of guard
  // and output the lhs and rhs conditions
  //
  bool getCurrentGuard(std::string x, arma::mat x_old, GiNaC::lst syms) {
    std::vector<bool> guard = {};
    std::vector<std::string> y;
    std::vector<std::string> y0;  /// = splitStr(x,'&');
    std::vector<std::string> y01; // = splitStr(x,'|');
    bool logAND = false;
    bool logOR = false;
    // Determine type of guard whether it contains:
    // ' ', '0', '>','<','<=','<=' or a combination
    if (x.size() == 1) // No action
    {
      guard.push_back(0); // stay same

    } else {
      std::vector<std::string> y = splitStr(x, ':');
      std::vector<std::string> y0 = splitStr(x, '&');
      std::vector<std::string> y01 = splitStr(x, '|');
      std::vector<std::string> xnew;
      int steps = 2; // TODO make a function of number of symbols
      if (y0.size() > 1 && y01.size() == 1) {
        steps = y0.size();
        xnew = y0;
        logAND = true;
      } else if (y0.size() == 1 && y01.size() > 1) {
        steps = y01.size();
        xnew = y01;
        logAND = true;
      } else {
        steps = 1; // y0.size()+y01.size();
        xnew = y0;
        //  xnew.push_back(y01[0]);//TODO FIX
        logAND = false;
        logOR = false;
      }
      for (int j = 0; j < steps; j++) {
        std::cout << "xold" << x_old(j, 0) << std::endl;

        std::vector<std::string> y1 = splitStr(xnew[j], '=');
        std::vector<std::string> y2 = splitStr(xnew[j], '>');
        std::vector<std::string> y3 = splitStr(xnew[j], '<');
        int totalSize = 0;
        if (y3[0].length() == x.length() && y2[0].length() != x.length() &&
            y1[0].length() != x.length())
          totalSize = y1.size() + y2.size();
        else if (y3[0].length() != x.length() && y2[0].length() == x.length() &&
                 y1[0].length() != x.length())
          totalSize = y1.size() + y3.size();
        else if (y3[0].length() != x.length() && y2[0].length() != x.length() &&
                 y1[0].length() == x.length())
          totalSize = y2.size() + y3.size();
        else
          totalSize = y1.size() + y2.size() + y3.size();
        // std::cout << "Total size: "<< totalSize <<std::endl;
        // Filter strings from logical operators
        for (unsigned int i = 0; i < y1.size(); i++) {
          // std::cout<< "Original expression: " << y1[i] << std::endl;
          int a = y1[i].find("x"), b = y1[i].find(">"), c = y1[i].find("<"),
              l = y1[i].length();

          if (a > -1 && (b > -1 || c > -1) && l > 3) {
            if (y1[i].find("x") == 0) {
              y1[i].erase(0, 3);
            } else {
              if (b > -1 && c > -1) {
                y1[i].erase(y1[i].find('>'), y1[i].length() - y1[i].find('>'));
                y1[i].erase(y1[i].find('<'), y1[i].length() - y1[i].find('<'));
              } else if (b == -1 && c > -1) {
                y1[i].erase(y1[i].find('<'), y1[i].length() - y1[i].find('<'));
              } else if (b > -1 && c == -1) {
                y1[i].erase(y1[i].find('>'), y1[i].length() - y1[i].find('>'));
              }
            }
          } else {
            y1[i].erase(std::remove(y1[i].begin(), y1[i].end(), '>'),
                        y1[i].end());
            y1[i].erase(std::remove(y1[i].begin(), y1[i].end(), '<'),
                        y1[i].end());
          }
          // std::cout<< "Clean expression: " << y1[i] << std::endl;
        }
        for (unsigned int i = 0; i < y2.size(); i++) {
          // std::cout<< "Original expression: " << y2[i] << std::endl;
          int index = y2[i].find("<=");
          int index2 = y2[i].find("x");
          if (index > -1) {
            if (index2 > index) {
              y2[i].erase(index, y2[i].length() - index);
            } else {
              y2[i].erase(0, index + 2);
            }
          }
          y2[i].erase(std::remove(y2[i].begin(), y2[i].end(), '='),
                      y2[i].end());
          // std::cout<< "Clean expression: " << y2[i] << std::endl;
        }
        for (unsigned int i = 0; i < y3.size(); i++) {
          // std::cout<< "Original expression: " << y3[i] << std::endl;
          int index = y3[i].find(">=");
          int index2 = y3[i].find("x");
          if (index > -1) {
            if (index2 > index) {
              y3[i].erase(index, y3[i].length() - index);
            } else {
              y3[i].erase(0, index + 2);
            }
          }
          y3[i].erase(std::remove(y3[i].begin(), y3[i].end(), '='),
                      y3[i].end());

          // std::cout<< "Clean expression: " << y3[i] << std::endl;
        }
        // To compute guard
        std::cout << syms[0] << std::endl;
        bool x1exist = y1[0].find("x1") != std::string::npos;
        bool x2exist = y1[0].find("x2") != std::string::npos;
        bool x1y2exist = y2[0].find("x1") != std::string::npos;
        bool x2y2exist = y2[0].find("x2") != std::string::npos;
        bool x1y3exist = y3[0].find("x1") != std::string::npos;
        bool x2y3exist = y3[0].find("x2") != std::string::npos;
        std::cout << x1exist << x2exist << x1y2exist << x2y2exist << x1y3exist
                  << x2y3exist << std::endl;
        switch (totalSize) {
        case 2: // = or > or <
        {
          if (y1.size() == 2) {
            GiNaC::ex ex1(y1[0], syms);
            GiNaC::ex ex2(y1[1], syms);
            GiNaC::ex lhs;
            GiNaC::ex rhs;
            if (x1exist) {
              lhs = ex1.subs(syms[0] == x_old(0, 0));
              rhs = ex2.subs(syms[0] == x_old(0, 0));
            } else {
              lhs = ex1.subs(syms[1] == x_old(1, 0));
              rhs = ex2.subs(syms[1] == x_old(1, 0));
            }
            guard.push_back((lhs == rhs));
            break;
          }
          if (y2.size() == 2) {
            GiNaC::ex ex1(y2[0], syms);
            GiNaC::ex ex2(y2[1], syms);
            GiNaC::ex lhs;
            GiNaC::ex rhs;
            if (x1y2exist) {
              lhs = ex1.subs(syms[0] == x_old(0, 0));
              rhs = ex2.subs(syms[0] == x_old(0, 0));
            } else {
              lhs = ex1.subs(syms[1] == x_old(1, 0));
              rhs = ex2.subs(syms[1] == x_old(1, 0));
            }
            guard.push_back(lhs > rhs);
            break;
          }
          if (y3.size() == 2) {
            GiNaC::ex ex1(y3[0], syms);
            GiNaC::ex ex2(y3[1], syms);
            GiNaC::ex lhs;
            GiNaC::ex rhs;
            if (x1y3exist) {
              lhs = ex1.subs(syms[0] == x_old(0, 0));
              rhs = ex2.subs(syms[0] == x_old(0, 0));
            } else {
              lhs = ex1.subs(syms[1] == x_old(1, 0));
              rhs = ex2.subs(syms[1] == x_old(1, 0));
            }
            guard.push_back(lhs < rhs);
            break;
          }
        } break;
        case 3: // a<x<b, a>x>b
          if (y2.size() == 3) {
            GiNaC::ex ex1(y2[0], syms);
            GiNaC::ex ex2(y2[1], syms);
            GiNaC::ex ex3(y2[2], syms);
            GiNaC::ex lhs;
            GiNaC::ex rhs;
            GiNaC::ex b;
            if (x1y2exist) {
              lhs = ex1.subs(syms[0] == x_old(0, 0));
              rhs = ex2.subs(syms[0] == x_old(0, 0));
              b = ex3.subs(syms[0] == x_old(0, 0));
            } else {
              lhs = ex1.subs(syms[1] == x_old(1, 0));
              rhs = ex2.subs(syms[1] == x_old(1, 0));
              b = ex3.subs(syms[1] == x_old(1, 0));
            }

            guard.push_back((lhs > rhs) || (rhs > b));
            break;
          }
          if (y3.size() == 3) {
            GiNaC::ex ex1(y3[0], syms);
            GiNaC::ex ex2(y3[1], syms);
            GiNaC::ex ex3(y3[2], syms);
            GiNaC::ex lhs;
            GiNaC::ex rhs;
            GiNaC::ex b;
            if (x1y3exist) {
              lhs = ex1.subs(syms[0] == x_old(0, 0));
              rhs = ex2.subs(syms[0] == x_old(0, 0));
              b = ex3.subs(syms[0] == x_old(0, 0));
            } else {
              lhs = ex1.subs(syms[1] == x_old(1, 0));
              rhs = ex2.subs(syms[1] == x_old(1, 0));
              b = ex3.subs(syms[1] == x_old(1, 0));
            }
            guard.push_back((lhs < rhs) || (rhs < b));
            break;
          }
          break;
        case 4: // a>=x, a<=x,a>x<b, a<x>b
          if (y1.size() == 2 && y2.size() == 2) {
            GiNaC::ex ex1(y1[0], syms);
            GiNaC::ex ex2(y1[1], syms);
            GiNaC::ex lhs;
            GiNaC::ex rhs;
            if (x1exist) {
              lhs = ex1.subs(syms[0] == x_old(0, 0));
              rhs = ex2.subs(syms[0] == x_old(0, 0));
            } else {
              lhs = ex1.subs(syms[1] == x_old(1, 0));
              rhs = ex2.subs(syms[1] == x_old(1, 0));
            }
            int where = x.find('x');
            int mid = x.length() / 2;

            std::cout << lhs << " " << rhs << std::endl;
            if (mid - where > 0)
              guard.push_back((lhs == rhs) || (lhs > rhs));
            else
              guard.push_back((rhs == rhs) || (rhs > lhs));
            break;
          }
          if (y1.size() == 2 && y3.size() == 2) {
            GiNaC::ex ex1(y1[0], syms);
            GiNaC::ex ex2(y1[1], syms);
            GiNaC::ex lhs;
            GiNaC::ex rhs;
            if (x1exist) {
              lhs = ex1.subs(syms[0] == x_old(0, 0));
              rhs = ex2.subs(syms[0] == x_old(0, 0));
            } else {
              std::cout << x_old(1, 0) << std::endl;
              lhs = ex1.subs(syms[1] == x_old(1, 0));
              rhs = ex2.subs(syms[1] == x_old(1, 0));
            }
            int where = x.find('x');
            int mid = x.length() / 2;
            bool one = lhs == rhs;
            bool two = (lhs < rhs);
            std::cout << one << " " << two << std::endl;
            if (mid - where > 0)
              guard.push_back((lhs == rhs) || (lhs < rhs));
            else
              guard.push_back((rhs == rhs) || (rhs < lhs));
            break;
          }
          if (y2.size() == 2 && y3.size() == 2) {
            GiNaC::ex ex1(y2[0], syms);
            GiNaC::ex ex2(y2[1], syms);
            GiNaC::ex ex3(y3[0], syms);
            GiNaC::ex ex4(y3[1], syms);

            GiNaC::ex lhs, b, c;
            GiNaC::ex rhs;
            if (x1y2exist) {
              lhs = ex1.subs(syms[0] == x_old(0, 0));
              rhs = ex2.subs(syms[0] == x_old(0, 0));
            } else {
              lhs = ex1.subs(syms[1] == x_old(1, 0));
              rhs = ex2.subs(syms[1] == x_old(1, 0));
            }
            if (x1y3exist) {
              b = ex3.subs(syms[0] == x_old(0, 0));
              c = ex4.subs(syms[0] == x_old(0, 0));
            } else {
              b = ex3.subs(syms[1] == x_old(1, 0));
              c = ex4.subs(syms[1] == x_old(1, 0));
            }
            guard.push_back((lhs > rhs) || (b < c));
            break;
          }
          break;
        case 5: // a>=x>b, a<=x<b,
          if (y1.size() == 2 && y2.size() == 1 && y3.size() == 2) {
            GiNaC::ex ex1(y1[0], syms);
            GiNaC::ex ex2(y1[1], syms);
            GiNaC::ex ex3(y3[0], syms);
            GiNaC::ex ex4(y3[1], syms);
            GiNaC::ex lhs, b, c;
            GiNaC::ex rhs;
            if (x1exist) {
              lhs = ex1.subs(syms[0] == x_old(0, 0));
              rhs = ex2.subs(syms[0] == x_old(0, 0));
            } else {
              lhs = ex1.subs(syms[1] == x_old(1, 0));
              rhs = ex2.subs(syms[1] == x_old(1, 0));
            }
            if (x1y3exist) {
              b = ex3.subs(syms[0] == x_old(0, 0));
              c = ex4.subs(syms[0] == x_old(0, 0));
            } else {
              b = ex3.subs(syms[1] == x_old(1, 0));
              c = ex4.subs(syms[1] == x_old(1, 0));
            }
            bool a = (lhs == rhs), d = (b < c);
            std::cout << "==" << a << " <" << d << std::endl;
            guard.push_back((lhs == rhs) || (b < c));
          }
          if (y1.size() == 2 && y2.size() == 2 && y3.size() == 1) {
            GiNaC::ex ex1(y1[0], syms);
            GiNaC::ex ex2(y1[1], syms);
            GiNaC::ex ex3(y2[0], syms);
            GiNaC::ex ex4(y2[1], syms);
            GiNaC::ex lhs, b, c;
            GiNaC::ex rhs;
            if (x1exist) {
              lhs = ex1.subs(syms[0] == x_old(0, 0));
              rhs = ex2.subs(syms[0] == x_old(0, 0));
            } else {
              lhs = ex1.subs(syms[1] == x_old(1, 0));
              rhs = ex2.subs(syms[1] == x_old(1, 0));
            }
            if (x1y2exist) {
              b = ex3.subs(syms[0] == x_old(0, 0));
              c = ex4.subs(syms[0] == x_old(0, 0));
            } else {
              b = ex3.subs(syms[1] == x_old(1, 0));
              c = ex4.subs(syms[1] == x_old(1, 0));
            }
            guard.push_back((lhs == rhs) || (b > c));
          }
          if (y1.size() == 2 && y2.size() == 3) {
            GiNaC::ex ex1(y1[0], syms);
            GiNaC::ex ex2(y1[1], syms);
            GiNaC::ex ex3(y2[0], syms);
            GiNaC::ex ex4(y2[1], syms);
            GiNaC::ex ex5(y2[2], syms);
            GiNaC::ex lhs, b, c;
            GiNaC::ex rhs, d;
            if (x1exist) {
              lhs = ex1.subs(syms[0] == x_old(0, 0));
              rhs = ex2.subs(syms[0] == x_old(0, 0));
            } else {
              lhs = ex1.subs(syms[1] == x_old(1, 0));
              rhs = ex2.subs(syms[1] == x_old(1, 0));
            }
            if (x1y2exist) {
              b = ex3.subs(syms[0] == x_old(0, 0));
              c = ex5.subs(syms[0] == x_old(0, 0));
              d = ex4.subs(syms[0] == x_old(0, 0));
            } else {
              b = ex3.subs(syms[1] == x_old(1, 0));
              c = ex5.subs(syms[1] == x_old(1, 0));
              d = ex4.subs(syms[1] == x_old(1, 0));
            }
            int where = x.find('=');
            int mid = x.length() / 2;
            if (mid - where > 0)
              guard.push_back((lhs == c) || (b > c) || (c > d));
            else
              guard.push_back((rhs == c) || (b > c) || (c > d));
            break;
          }
          if (y1.size() == 2 && y3.size() == 3) {
            GiNaC::ex ex1(y1[0], syms);
            GiNaC::ex ex2(y1[1], syms);
            GiNaC::ex ex3(y3[0], syms);
            GiNaC::ex ex4(y3[1], syms);
            GiNaC::ex ex5(y3[2], syms);
            GiNaC::ex lhs, b, c;
            GiNaC::ex rhs, d;
            if (x1exist) {
              lhs = ex1.subs(syms[0] == x_old(0, 0));
              rhs = ex2.subs(syms[0] == x_old(0, 0));
            } else {
              lhs = ex1.subs(syms[1] == x_old(1, 0));
              rhs = ex2.subs(syms[1] == x_old(1, 0));
            }
            if (x1y3exist) {
              b = ex3.subs(syms[0] == x_old(0, 0));
              c = ex5.subs(syms[0] == x_old(0, 0));
              d = ex4.subs(syms[0] == x_old(0, 0));
            } else {
              b = ex3.subs(syms[1] == x_old(1, 0));
              c = ex5.subs(syms[1] == x_old(1, 0));
              d = ex4.subs(syms[1] == x_old(1, 0));
            }
            int where = x.find('=');
            int mid = x.length() / 2;
            if (mid - where > 0)
              guard.push_back((lhs == c) || (b < c) || (c < d));
            else
              guard.push_back((rhs == c) || (b < c) || (c < d));
            break;
          }
          break;
        case 6: // a<=x>b, a>x<=b,a>=x<b,a<x>=b
        {
          GiNaC::ex ex1(y1[0], syms);
          GiNaC::ex ex2(y1[1], syms);
          GiNaC::ex ex3(y2[0], syms);
          GiNaC::ex ex4(y2[1], syms);
          GiNaC::ex ex5(y3[0], syms);
          GiNaC::ex ex6(y3[1], syms);
          GiNaC::ex lhs, b, c;
          GiNaC::ex rhs, d, e;
          if (x1exist) {
            lhs = ex1.subs(syms[0] == x_old(0, 0));
            rhs = ex2.subs(syms[0] == x_old(0, 0));
          } else {
            lhs = ex1.subs(syms[1] == x_old(1, 0));
            rhs = ex2.subs(syms[1] == x_old(1, 0));
          }
          if (x1y2exist) {
            b = ex3.subs(syms[0] == x_old(0, 0));
            c = ex4.subs(syms[0] == x_old(0, 0));
          } else {
            b = ex3.subs(syms[1] == x_old(1, 0));
            c = ex4.subs(syms[1] == x_old(1, 0));
          }
          if (x1y3exist) {
            d = ex5.subs(syms[0] == x_old(0, 0));
            e = ex6.subs(syms[0] == x_old(0, 0));
          } else {
            d = ex5.subs(syms[1] == x_old(1, 0));
            e = ex6.subs(syms[1] == x_old(1, 0));
          }
          int where = x.find('=');
          int mid = x.length() / 2;
          if (mid - where > 0)
            guard.push_back((lhs == c) || (b > c) || (c < d));
          else
            guard.push_back((rhs == c) || (b > c) || (c < d));
          break;
        }

        case 7: // a>=x<=b, a<=x>=b
        {
          GiNaC::ex ex1(y1[0], syms);
          GiNaC::ex ex2(y1[1], syms);
          GiNaC::ex ex2a(y1[2], syms);
          GiNaC::ex ex3(y2[0], syms);
          GiNaC::ex ex4(y2[1], syms);
          GiNaC::ex ex5(y3[0], syms);
          GiNaC::ex ex6(y3[1], syms);
          GiNaC::ex lhs, a, b, c;
          GiNaC::ex rhs, d, e;
          if (x1exist) {
            lhs = ex1.subs(syms[0] == x_old(0, 0));
            rhs = ex2.subs(syms[0] == x_old(0, 0));
            a = ex2a.subs(syms[0] == x_old(0, 0));
          } else {
            lhs = ex1.subs(syms[1] == x_old(1, 0));
            rhs = ex2.subs(syms[1] == x_old(1, 0));
            a = ex2a.subs(syms[1] == x_old(0, 0));
          }
          if (x1y2exist) {
            b = ex3.subs(syms[0] == x_old(0, 0));
            c = ex4.subs(syms[0] == x_old(0, 0));
          } else {
            b = ex3.subs(syms[1] == x_old(1, 0));
            c = ex4.subs(syms[1] == x_old(1, 0));
          }
          if (x1y3exist) {
            d = ex5.subs(syms[0] == x_old(0, 0));
            e = ex6.subs(syms[0] == x_old(0, 0));
          } else {
            d = ex5.subs(syms[1] == x_old(1, 0));
            e = ex6.subs(syms[1] == x_old(1, 0));
          }
          guard.push_back((lhs == rhs) || (rhs == a) || (b > c) || (c < d));
          break;
        }
        default: {
          guard.push_back(0);
          break;
        }
        }
      }
    }
    bool answer;
    std::cout << guard[0] << ", " << guard[1] << std::endl;
    if (logAND && logOR) // TODO FIX AND GENERALISE
    {
      answer = guard[0] && guard[1];
    } else if (logAND && !logOR) {
      answer = guard[0] && guard[1];
    }
    // else if(!logAND && logOR)
    // {
    else {
      answer = guard[0] || guard[1];
    }
    //}
    // else
    //{
    // answer = guard[0];
    // }

    return answer;
  }

  int checkData() {
    int error = 0;
    if ((unsigned)this->x_mod[0].x_dim != (unsigned)this->x_data.X[0].n_rows) {
      error = 1;
    }
    if ((unsigned)this->x_mod[0].u_dim != (unsigned)this->x_data.U.n_cols &&
        this->x_mod[0].u_dim > 0) {
      error = 2;
    }
    if ((unsigned)this->x_mod[0].d_dim != (unsigned)this->x_data.D.n_cols &&
        this->x_mod[0].d_dim > 0) {
      error = 3;
    }
    return error;
  }
  int NumSymbols(shs_t &old) {
    // To find how many symbolic continuous variables are needed
    // find string with maximum length
    int kmax = 0;
    for (int k = 0; k < pow(old.Q, 2); k++) {
      if (sizeof(old.Tq[k]) > sizeof(old.Tq[kmax])) {
        kmax = k;
      }
    }
    // std::cout << "kmax: " << kmax << std::endl;
    return kmax;
  }
  void populateTq(matvar_t &content) {
    ssmodels_t container;

    // Tq can be of two version
    // Cells containing strings for guards or
    // Numeric containing probabilities
    if (content.data != NULL) {
      std::cout << "Tq: " << content.data_type << std::endl;
      // Reading from cells
      std::string str = container.readCells(content);
      std::cout << "Tq: " << str << std::endl;
      std::vector<std::string> x = splitStr(str, ' '); // TODO: CHANGEseparater
      int numEl = x.size();
      std::cout << numEl << std::endl;
      int q = sqrt(numEl);
      std::cout << "modes: " << q << std::endl;
      this->Q = q;
      this->Tq = x; // Recall being stored in column format
    } else {
      std::cout << "Tq field not input in arma::mat file" << std::endl;
    }
  }
  std::vector<int> t_q(shs_t &old, int q_old, int monte) {
    int steps = 1;
    std::vector<int> modes(steps + 1);

    modes[0] = q_old;
    // if(old.x_mod[0].sigma.isZero(0) )
    //{
    std::random_device rd;
    std::mt19937 gen(rd());
    int count;
    double sum, U;
    for (int i = 0; i < steps; i++) {
      count = 0;
      sum = 0;
      U = std::generate_canonical<double, 10>(gen);
      while (sum < U) {
        sum += old.p_k[monte](modes[i], count);
        if (sum > U) {
          modes[i + 1] = count;
        }
        count++;
      }
    }

    return modes;
  }
};

/*************************************************************************************************/
// Single initial state SHS with fixed or sigmoidal probabilities governing the
// transitioning between modes`
template <> class shs_t<arma::mat, int> {
public:
  int Q;        // Discrete modes
  arma::mat Tq; // Discrete kernel- can represent either guards or probabilities
  int n;        // Dimension of continuous state space in given mode
  arma::mat p_k;
  arma::mat q_0;                 // Modes
  std::vector<ssmodels_t> x_mod; // container of models
  exdata_t x_data;

private:
  bool sigmoid;

public:
  shs_t() {
    n = 1; // Dimension of continuous state space in given mode
    p_k = arma::mat(Q, n);
    q_0 = arma::mat(Q, 1); // Initial mode
    std::vector<ssmodels_t> x_mod;
    Q = 1; // Discrete modes
    exdata_t x_data;
    Tq = arma::mat(n, n);
    sigmoid = false;
  }
  shs_t(int disc, int num, arma::mat tq, std::vector<ssmodels_t> &old,
        exdata_t &data) {
    Q = disc; // Discrete modes
    n = num;  // Dimension of continuous state space in given mode
    p_k = data.InitTq[0];
    q_0 = data.q_init[0]; // Initial mode
    x_mod = old;
    x_data = data;
    Tq = tq;
    sigmoid = false;
    try {
      // Check dimensions of data
      int err = this->checkData();
      switch (err) {
      case 3: {
        throw "Incorrect size of disturbance vector";
      }
      default: { std::cout << "Correct input vectors size" << std::endl; }
      }
    } catch (const char *msg) {
      std::cerr << msg << std::endl;
      exit(0);
    }
  }
  shs_t(int num, std::vector<ssmodels_t> &old, exdata_t &data) {
    Q = data.U.n_cols; // Discrete modes
    n = num;           // Dimension of continuous state space in given mode
    p_k = data.InitTq[0];
    q_0 = data.q_init[0]; // Initial mode
    x_mod = old;
    x_data = data;
    Tq = arma::zeros<arma::mat>(1, 1);
    Tq.reset();
    sigmoid = false;
    // update U dimension
    for (size_t i = 0; i < Q; i++) {
      old[i].u_dim = data.U.n_cols;
    }
    try {
      // Check dimensions of data
      int err = this->checkData();
      switch (err) {
      case 3: {
        throw "Incorrect size of disturbance vector";
      }
      default: { std::cout << "Correct input vectors size" << std::endl; }
      }
    } catch (const char *msg) {
      std::cerr << msg << std::endl;
      exit(0);
    }
  }
  shs_t(int num, std::vector<ssmodels_t> &old) {
    Q = old.size(); // Discrete modes
    n = num;        // Dimension of continuous state space in given mode
    p_k = arma::zeros<arma::mat>(1, 1);
    q_0 = arma::zeros<arma::mat>(1, 1); // Initial mode
    x_mod = old;
    exdata_t data;
    x_data = data;
    Tq = arma::zeros<arma::mat>(1, 1);
    Tq.reset();
    sigmoid = false;
  }
  shs_t(std::vector<ssmodels_t> &old) {
    Q = old.size();      // Discrete modes
    n = old[0].A.n_cols; // Dimension of continuous state space in given mode
    p_k = arma::zeros<arma::mat>(1, 1);
    q_0 = arma::zeros<arma::mat>(1, 1); // Initial mode
    x_mod = old;
    exdata_t data;
    x_data = data;
    Tq = arma::zeros<arma::mat>(1, 1);
    Tq.reset();
    sigmoid = false;
  }
  shs_t(arma::mat tq, std::vector<ssmodels_t> &old, exdata_t &data) {
    Q = tq.n_rows;    // Discrete modes
    n = old[0].x_dim; // Dimension of continuous state space in given mode
    p_k = data.InitTq[0];
    q_0 = data.q_init[0]; // Initial mode
    x_mod = old;
    x_data = data;
    Tq = tq;
    Tq = checkStochasticity(Tq);
    sigmoid = false;
    for (size_t i = 0; i < Q; i++) {
      old[i].u_dim = data.U.n_cols;
    }
    try {
      // Check dimensions of data
      int err = this->checkData();
      switch (err) {
      case 3: {
        throw "Incorrect size of disturbance vector";
      }
      default: { std::cout << "Correct input vectors size" << std::endl; }
      }
    } catch (const char *msg) {
      std::cerr << msg << std::endl;
      exit(0);
    }
  }
  shs_t(arma::mat tq, std::vector<ssmodels_t> &old) {
    Q = tq.n_rows;    // Discrete modes
    n = old[0].x_dim; // Dimension of continuous state space in given mode
    p_k = arma::zeros<arma::mat>(1, 1);
    q_0 = arma::zeros<arma::mat>(1, 1); // Initial mode
    x_mod = old;
    exdata_t data;
    x_data = data;
    Tq = tq;
    Tq = checkStochasticity(Tq);
    sigmoid = false;
    try {
      // Check dimensions of data
      int err = this->checkData();
      switch (err) {
      case 3: {
        throw "Incorrect size of disturbance vector";
      }
      default: { std::cout << "Correct input vectors size" << std::endl; }
      }
    } catch (const char *msg) {
      std::cerr << msg << std::endl;
      exit(0);
    }
  }
  shs_t(const char *fn, exdata_t &data) {
    // Obtain number of discrete modes and obtain Tq
    bool error = this->obtainTqfromMat(fn, *this);

    if (!error) {
      Q = this->Q;
      Tq = this->Tq;
      sigmoid = false;
      x_data = data;
      p_k = data.InitTq[0];
      q_0 = data.q_init[0]; // Initial mode
      // create array containing the ssmodels
      std::vector<ssmodels_t> models;
      arma::mat dummy = -1 * arma::ones<arma::mat>(1, 1);
      for (int i = 1; i <= Q; i++) {
        int x_dimension = data.x_k.n_rows;
        int u_dimension = data.u_k.n_rows;
        int d_dimension = data.d_k.n_rows;
        if (u_dimension == 1 && d_dimension == 1) {
          double lhs = arma::accu(data.u_k == dummy);
          double rhs = arma::accu(data.d_k == dummy);
          if ((lhs > 0) && (rhs > 0)) {
            u_dimension = 0;
            d_dimension = 0;
          } else if ((lhs > 0) && (rhs == 0)) {
            u_dimension = 0;
          }
        }
        ssmodels_t mod(x_dimension, u_dimension, d_dimension);
        if ((mod.C.n_rows == 1) && (mod.C.n_cols == 1) && x_dimension > 1) {
          mod.C.resize(x_dimension, x_dimension);
          mod.C = arma::eye<arma::mat>(x_dimension, x_dimension);
        }
        this->n = x_dimension; // TODO: update to handle different dimensions
                               // for each state (Store)
        // Reading models
        std::cout
            << "Initialising model of continuous variables in discrete mode "
            << i << std::endl;
        mod.obtainSSfromMat(fn, mod, i);
        if ((unsigned)x_dimension != (unsigned)mod.x_dim ||
            (unsigned)x_dimension != (unsigned)mod.A.n_rows) {
          mod.obtainSSfromMat(fn, mod);
        }
        mod.checkModel(mod);
        models.push_back(mod);
      }
      x_mod = models;
      n = models[0].x_dim;

      // Check dimensions of data
      int err = this->checkData();
      switch (err) {
      case 3: {
        std::cout << "Incorrect size of disturbance vector" << std::endl;
        exit(0);
      }
      default: { std::cout << "Correct input vectors size" << std::endl; }
      }
    } else {
      std::cout << "File " << fn << " not found." << std::endl;
      exit(0);
    }
  }
  // generate shs from file
  shs_t(const char *fn) {
    // Obtain number of discrete modes and obtain Tq
    bool error = this->obtainTqfromMat(fn, *this);
    if (!error) {
      Q = this->Q;
      Tq = this->Tq;
      sigmoid = false;
      // create array containing the ssmodels
      std::vector<ssmodels_t> models;
      arma::mat dummy = -1 * arma::ones<arma::mat>(1, 1);
      for (int i = 1; i <= Q; i++) {

        ssmodels_t mod;
        // Reading models
        std::cout
            << "Initialising model of continuous variables in discrete mode "
            << i << std::endl;
        mod.obtainSSfromMat(fn, mod, i);
        this->n = mod.A.n_rows; // TODO: update to handle different dimensions
                                // for each state (Store)

        mod.checkModel(mod);
        models.push_back(mod);
      }
      x_mod = models;
      n = models[0].x_dim;
    } else {
      std::cout << "File " << fn << " not found." << std::endl;
      exit(0);
    }
  }
  shs_t(const char *fn, int bmdp, int modes) {
    if (bmdp == 1) {
      std::vector<ssmodels_t> models;
      for (int i = 1; i <= modes; i++) {
        ssmodels_t mod(1, 1, 1);
        mod.obtainBMDPfromMat(fn, mod, i);
        mod.x_dim = mod.A.n_rows;
        mod.u_dim = 0;
        mod.d_dim = 0;
        models.push_back(mod);
      }
      this->x_mod = models;
      this->n = models[0].x_dim;
    } else {
      // Obtain number of discrete modes and obtain Tq
      bool error = this->obtainTqfromMat(fn, *this);
      if (!error) {
        Q = this->Q;
        Tq = this->Tq;
        sigmoid = false;
        // create array containing the ssmodels
        std::vector<ssmodels_t> models;
        arma::mat dummy = -1 * arma::ones<arma::mat>(1, 1);
        for (int i = 1; i <= Q; i++) {
          int x_dimension = this->x_mod[i].A.n_rows;
          int u_dimension = this->x_mod[i].B.n_rows;
          int d_dimension = this->x_mod[i].F.n_rows;
          ssmodels_t mod(x_dimension, u_dimension, d_dimension);
          this->n = x_dimension; // TODO: update to handle different dimensions
                                 // for each state (Store)
          // Reading models
          mod.obtainSSfromMat(fn, mod, i);
          if ((unsigned)x_dimension != (unsigned)mod.x_dim ||
              (unsigned)x_dimension != (unsigned)mod.A.n_rows) {
            mod.obtainSSfromMat(fn, mod);
          }
          mod.checkModel(mod);
          models.push_back(mod);
        }
        x_mod = models;
        n = models[0].x_dim;
      } else {
        std::cout << "File " << fn << " not found." << std::endl;
        exit(0);
      }
    }
  }
  shs_t(const char *fn, exdata_t &data, int NumModes) {
    // Obtain number of discrete modes and obtain Tq
    Q = NumModes;
    sigmoid = true;
    Tq = arma::eye<arma::mat>(NumModes, NumModes);
    x_data = data;
    p_k = data.InitTq[0];
    q_0 = data.q_init[0]; // Initial mode
    // create array containing the ssmodels
    std::vector<ssmodels_t> models;
    arma::mat dummy = -1 * arma::ones<arma::mat>(1, 1);
    for (int i = 1; i <= Q; i++) {
      int x_dimension = data.x_k.n_rows;
      int u_dimension = data.u_k.n_rows;
      int d_dimension = data.d_k.n_rows;
      double lhs = arma::accu(data.u_k == dummy);
      double rhs = arma::accu(data.d_k == dummy);
      if ((lhs > 0) && (rhs > 0)) {
        u_dimension = 0;
        d_dimension = 0;
      } else if ((lhs > 0) && (rhs == 0)) {
        u_dimension = 0;
      }
      ssmodels_t mod(x_dimension, u_dimension, d_dimension);
      this->n = x_dimension; // TODO: update to handle different dimensions for
                             // each state (Store)
      // Reading models
      mod.obtainSSfromMat(fn, mod, i);
      if ((unsigned)x_dimension != (unsigned)mod.x_dim ||
          (unsigned)x_dimension != (unsigned)mod.A.n_rows) {
        mod.obtainSSfromMat(fn, mod);
      }
      mod.checkModel(mod);
      models.push_back(mod);
    }
    x_mod = models;

    try {
      // Check dimensions of data
      int err = this->checkData();
      switch (err) {
      case 3: {
        throw "Incorrect size of disturbance vector";
      }
      default: { throw "Correct input vectors size"; }
      }
    } catch (const char *msg) {
      std::cerr << msg << std::endl;
      exit(0);
    }
  }
  virtual ~shs_t() {}
  std::vector<int> t_q(shs_t &old, int q_old) {
    int steps = 1;
    std::vector<int> modes(steps + 1);

    modes[0] = q_old;
    std::random_device rd;
    std::mt19937 gen(rd());
    int count;
    double sum, U;
    for (int i = 0; i < steps; i++) {
      count = 0;
      sum = 0;
      U = std::generate_canonical<double, 10>(gen);
      while (sum < U) {
        sum += old.p_k(modes[i], count);
        if (sum > U) {
          modes[i + 1] = count;
        }
        count++;
      }
    }

    return modes;
  }
  // Dynamic model of continuous state
  void step(shs_t &old, int n, int steps) {
    arma::mat x_old(old.x_data.X[n]);
    arma::mat x_new = arma::mat(old.x_mod[0].x_dim, steps);
    arma::mat u_old = old.x_data.u_k;
    arma::mat d_old = arma::mat(old.x_mod[(int)old.q_0(n, 0)].d_dim, steps);
    d_old = old.x_data.d_k;
    if (old.Q == 1) {
      if (old.x_mod[0].N.is_empty()) {
        if(!u_old.is_empty()){
          if(u_old.n_cols == old.x_mod[0].B.n_rows){
            u_old = u_old.t();
          }
          u_old = arma::repmat(old.x_data.u_k.col(n),1,steps);
        }
        if(!d_old.is_empty()){
          if(d_old.n_cols == old.x_mod[0].F.n_rows){
            d_old = d_old.t();
          }
          d_old = arma::repmat(d_old.col(n),1,steps);
        }
        x_new = old.x_mod[0].updateLTIst(old.x_mod[0], x_old, u_old, d_old);
      } else {
        x_new =
            old.x_mod[0].updateBi(old.x_mod[0].A, old.x_mod[0].B,
                                  old.x_mod[0].N, old.x_mod[0].Q, x_old, u_old);
      }
    } else {
      double lhs =
          arma::accu(old.x_mod[(int)old.q_0(n, 0)].sigma ==
                     arma::zeros<arma::mat>(
                         (double)old.x_mod[(int)old.q_0(n, 0)].sigma.n_rows,
                         (double)old.x_mod[(int)old.q_0(n, 0)].sigma.n_cols));
      if (lhs > 0) {
        for (int j = 0; j < steps; j++) {
          if (old.sigmoid) {
            // Generalise: assumes for now that only have pairs of discrete
            // modes
            old.p_k(0, 0) = sigmoidCompute(x_old(0, 0), 100, 19.5);
            old.p_k(0, 1) = 1 - old.p_k(0, 0);
            old.p_k(1, 1) = 1 - old.p_k(0, 0);
            old.p_k(1, 0) = old.p_k(1, 1);
            std::cout << "Trans: " << old.p_k << std::endl;
          } else {
            if (n > 0 && !old.Tq.is_empty()) {
              old.p_k = old.p_k * old.Tq;
            }
          }
          if (!old.Tq.is_empty()) {
            int q_0 = (int)old.q_0(n, j);
            std::vector<int> q = t_q(old, q_0);
            int q_new = q[1];

            if (!old.x_mod[(int)old.q_0(n, j)].N.is_empty()) {
              x_new = old.x_mod[q_new].updateBi(old.x_mod[(int)old.q_0(n, j)].A,
                                                old.x_mod[(int)old.q_0(n, j)].B,
                                                old.x_mod[(int)old.q_0(n, j)].N,
                                                old.x_mod[old.q_0(n, j)].Q,
                                                x_old, u_old);
            } else if (old.x_mod[(int)old.q_0(n, j)].d_dim == 0) {
              x_new.col(j) = old.x_mod[q_new].updateLTI(
                  old.x_mod[(int)old.q_0(n, j)], x_old.col(j), u_old.col(j));
            } else {
              x_new.col(j) = old.x_mod[q_new].updateLTIad(
                  old.x_mod[(int)old.q_0(n, j)], x_old.col(j), u_old.col(j), d_old.col(j));
            }
            // Append result to Q in old object (columns)
            old.q_0(n + 1, j) = q_new;
          } else {
            int q_new = old.x_data.U(n + 1, 0);
            arma::mat emptyMat = arma::zeros<arma::mat>(1, 1);
            emptyMat.reset();
            x_new = old.x_mod[q_new].updateLTIst(
                old.x_mod[(int)u_old(0, 0)], x_old.col(j), emptyMat, emptyMat);
            // Append result to Q in old object (columns)
            old.q_0(n + 1, 0) = q_new;
          }
        }

      } else {
        // Get next state by updating p_k
        // Assumes continuous variables are affected by white noise
        // described using gaussian distribution with mean x[k] = x_old,
        // variance sigma
        // Compute conditional distribution for each mode
        for (int j = 0; j < steps; j++) {
          int q_0 = (int)old.q_0(n, j);
          if (old.sigmoid) {
            old.p_k(0, 0) = sigmoidCompute(x_old(0, 0), 1, 19.5) *
                            sigmoidCompute(x_old(1, 0), 1, 21.25);
            old.p_k(0, 1) = 1 - old.p_k(0, 0);
            old.p_k(1, 1) = old.p_k(0, 0);
            old.p_k(1, 0) = 1 - old.p_k(0, 0);
          } else {
            computeConditional(old, x_old, j, q_0);
          }
          // Sample from conditional distribution to get q_new
          std::vector<int> q = t_q(old, q_0);
          int q_new = q[1];
          // Append result to Q in old object (columns)
          old.q_0(n + 1, j) = q_new;
          // Assuming no reset kernels
          arma::mat x_up = arma::mat(old.x_mod[q_new].x_dim, 1);
          if (!old.x_mod[(int)old.q_0(n, j)].N.is_empty()) {
            x_up = old.x_mod[q_new].updateBi(old.x_mod[(int)old.q_0(n, j)].A,
                                             old.x_mod[(int)old.q_0(n, j)].B,
                                             old.x_mod[(int)old.q_0(n, j)].N,
                                             old.x_mod[old.q_0(n, j)].Q, x_old,
                                             u_old);
            for (int k = 0; k < old.x_mod[q_new].x_dim; k++) {
              x_new(k, j) =
                  getSampleNormal(x_up(k, 0), old.x_mod[q_new].sigma(k, 0));
            }

          } else if (old.x_mod[(int)old.q_0(n, j)].d_dim == 0) {

            x_up = old.x_mod[q_new].updateLTI(old.x_mod[(int)old.q_0(n, j)],
                                              x_old.col(j), u_old.col(j));
            for (int k = 0; k < old.x_mod[q_new].x_dim; k++) {
              x_new(k, j) =
                  getSampleNormal(x_up(k, 0), old.x_mod[q_new].sigma(k, 0));
            }
          } else {
            x_up = old.x_mod[q_new].updateLTIad(old.x_mod[(int)old.q_0(n, j)],
                                                x_old.col(j), u_old.col(j), d_old.col(j));
            for (int k = 0; k < old.x_mod[q_new].x_dim; k++) {
              x_new(k, j) =
                  getSampleNormal(x_up(k, 0), old.x_mod[q_new].sigma(k, 0));
            }
          }
        }
      }
    }
    // Append result to X in old object (3rd dimension)
    old.x_data.X.push_back(x_new);
  }
  void computeConditional(shs_t &old, arma::mat x_old, int index, int q_old) {
    double min = -1, max = 1;
    double val; //,err, xmin[1] ={}, xmax[1]={};
    std::vector<double> v = {};
    for (int i = 0; i < 2 * old.x_mod[q_old].x_dim; i++) {
      if (i < old.x_mod[q_old].x_dim) {
        v.push_back(x_old(i, index)); //
      } else {
        // TODO: Case when sigma is correlated
        v.push_back(old.x_mod[q_old].sigma(i - old.x_mod[q_old].x_dim, 0));
      }
    }
    val = 1;
    // Update p_k
    if (n == 0) {
      old.p_k = old.Tq;
    } else {
      old.p_k = old.p_k * val * old.Tq;
    }
    //    std::cout << "New kernel: " << old.p_k << std::endl;
  }
  void createPySimPlots(arma::mat y, arma::mat modes, int x_dim) {
    int T = y.n_rows / x_dim;
      std::cout << "Creating python simulation plot file" << std::endl;
      std::ofstream myfile;
      // If folder does not already exist then
      // create it else store in already existant
      // folder
      if(checkFolderExists("../results") == -1) {
        if(mkdir("../results", 0777) == -1) {
           std::cerr << "Error cannot create results directory: " <<std::strerror(errno) <<std::endl;
           exit(0);
        }
        else {
          std::cout <<" Results direcrtory created at ../results" <<std::endl;
        }
      }

      myfile.open("../results/simPlots.py");

      myfile << "from mpl_toolkits.mplot3d import Axes3D" << std::endl;
      myfile << "import numpy as np" << std::endl;
      myfile << "import matplotlib.pyplot as plt" << std::endl;
      myfile << std::endl;

      // Get separate cont var evolution
      std::vector<arma::mat> y_all;
      arma::mat y1 = arma::zeros<arma::mat>(T, y.n_cols);

      if (x_dim == 1) {
        y1 = y;
      } else {
        for(size_t j = 0; j < x_dim; j++){
          int count =0;
          for (unsigned i = j; i < y.n_rows; i = i + x_dim) {
            y1.row(count) = y.row(i);
            count++;
          }
          y_all.push_back(y1);
          y1 = arma::zeros<arma::mat>(T, y.n_cols);
        }
      }

      for (unsigned i = 0; i < x_dim; i++) {
        myfile << "x" << i << "= ["; // Creating variable for 1 trace
        for (unsigned j = i; j < y.n_rows; j = j + x_dim) {
          if (j == (y.n_rows - x_dim + i)) {
            myfile << y(j, 0) << "]" << std::endl;
          } else {
            myfile << y(j, 0) << ",";
          }
        }

        myfile << "plt.subplot("<<x_dim<< ",3," << i + 1 << ")" << std::endl;
        myfile << "plt.plot(x" << i << ")" << std::endl;
        myfile << "plt.title('Sample trace of continuous variable $x_" << i + 1
               << "$',fontsize=18)" << std::endl;
        myfile << "plt.xlabel('Time steps') " << std::endl;
        myfile << "plt.ylabel('Continuous variable $x_" << i + 1 << "$')"
               << std::endl;
        myfile << std::endl;
      }

      // Now we need to print the discrete modes
      myfile << "modes = ["; // Creating variable for mean values
      for (unsigned j = 0; j < T; j++) {
        if (j == (T - 1)) {
          myfile << modes(j, 0) << "]" << std::endl;
        } else {
          myfile << modes(j, 0) << ",";
        }
      }
      myfile << "plt.subplot("<<x_dim<< ",3," << x_dim + 1 << ")" << std::endl;
      myfile << "plt.plot(modes,marker='o',drawstyle='steps')" << std::endl;
      myfile << "plt.yticks(np.arange(min(modes),max(modes)+1,1))" << std::endl;
      myfile << "plt.title('Sample trace of discrete modes',fontsize=18)"
             << std::endl;
      myfile << "plt.xlabel('Time steps') " << std::endl;
      myfile << "plt.ylabel('Discrete modes')" << std::endl;
      myfile << std::endl;

      // Plotting of histograms
      int n_add = x_dim + 1; // Number of additional subplots

      // Defining time horizon
      for (unsigned i = 0; i < n_add; i++) {
        myfile << "ax = plt.subplot("<<x_dim<< ",3," << i + x_dim + 2
               << ", projection = '3d')" << std::endl;
        // Compute number of bins
        int binNo = 1;
        if (i < n_add -1) {
          myfile << "data_2d= [ ["; // Creating data set for historgram from y1
          for (unsigned p = 0; p < y_all[i].n_cols; p++) {
            for (unsigned j = 0; j < y_all[i].n_rows; j++) {
              if (j == (y_all[i].n_rows - 1)) {
                myfile << y_all[i](j, p) << "]," << std::endl;
              } else {
                myfile << y_all[i](j, p) << ",";
              }
            }
            if (p < (y_all[i].n_cols - 1)) {
              myfile << "[";
            }
          }
          int y1min = (int)arma::min(arma::min(y_all[i]));
          int y1max = (int)arma::max(arma::max(y_all[i]));
          double delta1 = (int)(y1max - y1min) / 5;
          if (delta1 == 0) {
            delta1 = 1;
          }

          myfile << "]" << std::endl;
          myfile << "data_array = np.array(data_2d)" << std::endl;
          myfile << std::endl;
          myfile << "# This is  the colormap I'd like to use." << std::endl;
          myfile << "cm = plt.cm.get_cmap('viridis')" << std::endl;
          myfile << std::endl;
          myfile << "#Create 3D histogram" << std::endl;
          myfile << "for z in range(" << T << "):" << std::endl;
          myfile << "     j = np.transpose(data_2d)" << std::endl;
          myfile << "     y_data = j[z][1:]" << std::endl;
          myfile << "     hist, bins = np.histogram(y_data,bins=75)"
                 << std::endl;
          myfile << "     center = (bins[:-1] + bins[1:])/2" << std::endl;
          myfile << "     x_span = bins.max() - bins.min()" << std::endl;
          myfile << "     C = [cm((x-bins.min())/x_span) for x in bins]"
                 << std::endl;
          myfile << "     ax.bar(center, hist, zs =z, zdir= 'y',color =C, fill "
                    "= 'true')"
                 << std::endl;
          myfile << "     ax.set_ylabel('Time steps') " << std::endl;
          myfile << "     ax.set_xlabel('Continuous variable $x_" << i + 1 << "$')"
                 << std::endl;
          myfile << "     ax.set_zlabel('Count')" << std::endl;
          myfile << "plt.ylim(" << 1 << "," << T << ")" << std::endl;
          myfile << "plt.xticks(np.arange(" << y1min << " ," << y1max
                 << ",step=" << delta1 << "))" << std::endl;
          myfile << std::endl;

        } else if (i == (n_add-1)) {
          myfile << "data_2d2= [ ["; // Creating data set for historgram from y1
          for (unsigned p = 0; p < modes.n_cols; p++) {
            for (unsigned j = 0; j < modes.n_rows; j++) {
              if (j == (modes.n_rows - 1)) {
                myfile << modes(j, p) << "]," << std::endl;
              } else {
                myfile << modes(j, p) << ",";
              }
            }
            if (p < (y1.n_cols - 1)) {
              myfile << "[";
            }
          }

          myfile << "]" << std::endl;
          int mmax = (int)arma::max(arma::max(modes));
          myfile << "data_array2 = np.array(data_2d2)" << std::endl;
          myfile << std::endl;
          myfile << "#Create 3D histogram" << std::endl;
          myfile << "for z2 in range(" << T << "):" << std::endl;
          myfile << "     j2 = np.transpose(data_2d2)" << std::endl;
          myfile << "     y_data2 = j2[z2][1:]" << std::endl;
          myfile << "     hist2, bins2 = np.histogram(y_data2,bins=" << mmax + 1
                 << ")" << std::endl;
          myfile << "     center2 = (bins2[:-1] + bins2[1:])/2" << std::endl;
          myfile << "     x_span2 = bins2.max() - bins2.min()" << std::endl;
          myfile << "     C = [cm((x2-bins2.min())/x_span2) for x2 in bins2]"
                 << std::endl;
          myfile << "     ax.bar(center2, hist2, zs =z2, zdir= 'y', fill = "
                    "'true', color=C)"
                 << std::endl;
          myfile << "     ax.set_ylabel('Time steps') " << std::endl;
          myfile << "     ax.set_xlabel('Discrete variable $q$')" << std::endl;
          myfile << "     ax.set_zlabel('Count')" << std::endl;
          myfile << "plt.ylim(" << 1 << "," << T << ")" << std::endl;
          myfile << "plt.xticks(np.arange(0," << mmax + 1 << ",step=1))"
                 << std::endl;

        }
    }
    myfile << "plt.savefig(\"SimulationRun.svg\",dpi=150)\n";
    myfile << "plt.show()" << std::endl;
    myfile.close();

  }
  void run(shs_t &old, int N, int steps) {
    clock_t begin, end;
    begin = clock();
    double time = 0;

    // For deterministic case perform 1 run
    // For stochastic version perform Monte Carlo + compute mean
    int i = 0, x_dim = old.x_mod[0].A.n_rows;

    arma::mat y = arma::zeros<arma::mat>(N * x_dim, steps);
    arma::mat modes = arma::zeros<arma::mat>(N, steps);
    old.p_k.set_size(size(old.Tq));
    old.p_k = old.Tq;
    int count = 0;
    while (i < N) {
      old.step(old, i, steps);
      if (old.x_mod[0].u_dim > 0) {
        if ((unsigned)old.x_data.U.n_rows == (unsigned)N) {
          old.x_data.u_k = old.x_data.U.row(i);
        }
      }
      if (old.x_mod[0].d_dim > 0) {
        if ((unsigned)old.x_data.D.n_rows == (unsigned)N) {
          old.x_data.d_k = old.x_data.D.row(i);
        }
      }
      arma::mat tempX = old.x_data.X[i];
      if(x_dim == 1) {
        y.row(count) = tempX;
      }
      else {
        y.rows(count, count + x_dim-1) = tempX;
      }
      modes.row(i) = old.q_0.row(i);
      count += x_dim;
      i++;
    }
    end = clock();
    time = (double)(end - begin) / CLOCKS_PER_SEC;
    std::ostringstream oss;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    oss << std::put_time(&tm, "%d-%m-%Y-%H-%M-%S");
    auto str = oss.str();

    std::cout << std::endl;
    std::cout << "--------------------------------------" <<std::endl;
    std::cout << " Simulation time                      " << std::endl;
    std::cout << "--------------------------------------" <<std::endl;
    std::cout << " " << time << std::endl;
    std::cout << "--------------------------------------" <<std::endl;
    std::cout << std::endl;

    // Option to export to file results
    std::ofstream myfile;
    std::string exportOpt;
    std::string str0("y");
    std::string str1("yes");
    std::cout << "Would you like to store simulation results [y- yes, n - no] " << std::endl;
    std::cin >> exportOpt;
    if ((exportOpt.compare(str0) == 0) || (exportOpt.compare(str1) == 0)) {
      // check if results folder exists:
      if(checkFolderExists("../results") == -1) {
        if(mkdir("../results", 0777) == -1) {
          std::cerr << "Error cannot create results directory: " <<std::strerror(errno) <<std::endl;
          exit(0);
         }
      }
      // Store results in file
      std::string f_name = "../results/Simulationtime_" + str + ".txt";
      myfile.open(f_name);
      myfile << time <<std::endl;
      myfile.close();
      std::string y_name = "../results/y_" + str + ".txt";
      y.save(y_name, arma::raw_ascii);
      arma::mat q = old.q_0;
      std::string q_name = "../results/modes_" + str + ".txt";
      q.save(q_name, arma::raw_ascii);
      old.createPySimPlots(y, modes, x_dim);
    }

  }
  bool obtainTqfromMat(const char *fn, shs_t &init) {
    // Reading model file input in .arma::mat format
    // and storing into ssmodel class
    mat_t *matf;
    matvar_t *matvar, *contents;
    // Read .mat file
    bool error = 0;

    matf = Mat_Open(fn, MAT_ACC_RDONLY);
    if (matf) // if successful in reading file
    {
      // read each variable within file and populate
      // state space model based on variable name
      contents = Mat_VarRead(matf, "Tq");
      if (contents == NULL) {
        init.Q = 1;
      } else {
        init.populateTq(*contents);
        contents = NULL;
      }
      Mat_Close(matf);

    } else // unsuccessfull in opening file
    {
      return 1;
    }
  }

private:
  int checkData() {
    int error = 0;
    if ((unsigned)this->x_mod[0].x_dim != (unsigned)this->x_data.X[0].n_rows) {
      error = 1;
    }
    if ((unsigned)this->x_mod[0].u_dim != (unsigned)this->x_data.U.n_cols &&
        (unsigned)this->x_mod[0].u_dim > 0) {
      error = 2;
    }
    if ((unsigned)this->x_mod[0].d_dim != (unsigned)this->x_data.D.n_cols &&
        (unsigned)this->x_mod[0].d_dim > 0) {
      error = 3;
    }
    return error;
  }
  void populateTq(matvar_t &content) {
    ssmodels_t container;

    // Tq can be of two version
    // Cells containing strings for guards or
    // Numeric containing probabilities
    if (content.data != NULL) {
      if (content.data_type == MAT_T_DOUBLE) {
        std::string str;
        size_t stride = Mat_SizeOf(content.data_type);
        char *data = (char *)content.data;
        unsigned i, j = 0;
        for (i = 0; i < content.dims[0]; i++) {
          for (j = 0; j < content.dims[1]; j++) {
            size_t idx = content.dims[0] * j + i;
            void *t = data + idx * stride;
            char substr[100];
            sprintf(substr, "%g",
                    *(double *)t); // Assumes values are of type double
            str.append(substr);
            str.append(" ");
          }
          str.append(";");
        }
        std::vector<std::string> x = splitStr(str, ';');
        int numEl = x.size();
        this->Q = numEl;

        // Check stochasticity of kernel
        arma::mat tq = strtodMatrix(x);
        tq = checkStochasticity(tq);
        this->Tq = tq;
      } else {
        std::cout << "Incorrect Tq format" << std::endl;
      }
    } else {
      std::cout << "Tq field not input in arma::mat file" << std::endl;
    }
  }
};

/*************************************************************************************************/
// SHS with multiple initial modes
template <> class shs_t<arma::mat, std::vector<int>> {
public:
  std::vector<int> Q; // Discrete modes
  std::vector<arma::mat>
      Tq; // Discrete kernel- can represent either guards or probabilities
  int n;  // Dimension of continuous state space in given mode
  std::vector<arma::mat> p_k;
  std::vector<arma::mat> q_0;    // Modes
  std::vector<ssmodels_t> x_mod; // container of models
  exdata_t x_data;

private:
  bool sigmoid;

public:
  shs_t() {
    Q = {1}; // Discrete modes
    n = 1;   // Dimension of continuous state space in given mode
    p_k = {arma::mat(Q[0], n)};
    q_0 = {arma::mat(Q[0], 1)}; // Initial mode
    std::vector<ssmodels_t> x_mod;
    exdata_t x_data;
    Tq = {arma::mat(n, n)};
    sigmoid = false;
  }
  shs_t(std::vector<int> disc, int num, std::vector<arma::mat> tq,
        std::vector<ssmodels_t> &old, exdata_t &data) {
    Q = disc; // Discrete modes
    n = num;  // Dimension of continuous state space in given mode
    p_k = data.InitTq;
    q_0 = data.q_init; // Initial mode
    x_mod = old;
    x_data = data;
    Tq = tq;
    sigmoid = false;
    try {
      // Check dimensions of data
      int err = this->checkData();
      switch (err) {
      case 3: {
        throw "Incorrect size of disturbance vector";
      }
      default: { throw "Correct input vectors size"; }
      }
    } catch (const char *msg) {
      std::cerr << msg << std::endl;
      exit(0);
    }
  }
  shs_t(std::vector<arma::mat> tq, std::vector<ssmodels_t> &old,
        exdata_t &data) {
    Q = {1};          // Discrete modes
    n = old[0].x_dim; // Dimension of continuous state space in given mode
    p_k = data.InitTq;
    q_0 = data.q_init; // Initial mode
    x_mod = old;
    x_data = data;
    Tq = tq;
    sigmoid = false;
    try {
      // Check dimensions of data
      int err = this->checkData();
      switch (err) {
      case 3: {
        throw "Incorrect size of disturbance vector";
      }
      default: { throw "Correct input vectors size"; }
      }
    } catch (const char *msg) {
      std::cerr << msg << std::endl;
      exit(0);
    }
  }
  shs_t(const char *fn, exdata_t &data, std::vector<int> NumModes) {
    // Obtain number of discrete modes and obtain Tq
    std::vector<int> num = {2, 2};

    Q = num;

    sigmoid = true;
    for (unsigned int m = 0; m < num.size(); m++) {
      Tq.push_back(arma::eye<arma::mat>(num[m], num[m]));
      p_k.push_back(data.InitTq[m]);
      q_0.push_back(data.q_init[m]); // Initial mode
    }
    x_data = data;

    // create array containing the ssmodels
    std::vector<ssmodels_t> models;
    arma::mat dummy = -1 * arma::ones<arma::mat>(1, 1);
    for (int i = 1; i <= Q[0]; i++) {
      int x_dimension = data.x_k.n_rows;
      int u_dimension = data.u_k.n_rows;
      int d_dimension = data.d_k.n_rows;
      double lhs = arma::accu(data.u_k == dummy);
      double rhs = arma::accu(data.d_k == dummy);
      if ((lhs > 0) && (rhs > 0)) {
        u_dimension = 0;
        d_dimension = 0;
      } else if ((lhs > 0) && (rhs == 0)) {
        u_dimension = 0;
      }
      ssmodels_t mod(x_dimension, u_dimension, d_dimension);
      this->n = x_dimension; // TODO: update to handle different dimensions for
                             // each state (Store)
      // Reading models
      mod.obtainSSfromMat(fn, mod, i);
      if ((unsigned)x_dimension != (unsigned)mod.x_dim ||
          (unsigned)x_dimension != (unsigned)mod.A.n_rows) {
        mod.obtainSSfromMat(fn, mod);
      }
      mod.checkModel(mod);
      models.push_back(mod);
    }
    x_mod = models;
    try {
      // Check dimensions of data
      int err = this->checkData();
      switch (err) {
      case 3: {
        throw "Incorrect size of disturbance vector";
      }
      default: { std::cout << "Correct input vectors size" << std::endl; }
      }
    } catch (const char *msg) {
      std::cerr << msg << std::endl;
      exit(0);
    }
  }

  shs_t(const char *fn, std::vector<int> NumModes) {
    // Obtain number of discrete modes and obtain Tq
    std::vector<int> num = {2, 2};

    Q = num;

    sigmoid = true;
    for (unsigned int m = 0; m < num.size(); m++) {
      Tq.push_back(arma::eye<arma::mat>(num[m], num[m]));
    }
    // create array containing the ssmodels
    std::vector<ssmodels_t> models;
    arma::mat dummy = -1 * arma::ones<arma::mat>(1, 1);
    for (int i = 1; i <= Q[0]; i++) {
      int x_dimension = this->x_mod[i].A.n_rows;
      int u_dimension = this->x_mod[i].B.n_rows;
      int d_dimension = this->x_mod[i].F.n_rows;
      ssmodels_t mod(x_dimension, u_dimension, d_dimension);
      this->n = x_dimension; // TODO: update to handle different dimensions for
                             // each state (Store)
      // Reading models
      std::cout
          << "Initialising model of continuous variables in discrete mode " << i
          << std::endl;
      mod.obtainSSfromMat(fn, mod, i);
      if ((unsigned)x_dimension != (unsigned)mod.x_dim ||
          (unsigned)x_dimension != (unsigned)mod.A.n_rows) {
        mod.obtainSSfromMat(fn, mod);
      }
      mod.checkModel(mod);
      models.push_back(mod);
    }
    x_mod = models;
  }

  virtual ~shs_t() {}
  std::vector<int> t_q(shs_t &old, int q_old, int index) {
    int steps = 1;
    std::vector<int> modes(steps + 1);

    modes[0] = q_old;
    // if(old.x_mod[0].sigma.isZero(0) )
    //{
    std::random_device rd;
    std::mt19937 gen(rd());
    int count;
    double sum, U;
    for (int i = 0; i < steps; i++) {
      count = 0;
      sum = 0;
      U = std::generate_canonical<double, 10>(gen);
      while (sum < U) {
        sum += old.p_k[index](modes[i], count);
        if (sum > U) {
          modes[i + 1] = count;
        }
        count++;
      }
    }

    return modes;
  }
  // Dynamic model of continuous state
  void step(shs_t &old, int n, int steps) {
    arma::mat x_old(old.x_data.X[n]);
    // arma::mat x_old = arma::mat(old.x_mod[(int) old.q_0[0](n,0)].x_dim,1);
    // x_old = old.x_data.X[n];
    arma::mat x_new = arma::mat(old.x_mod[(int)old.q_0[0](n, 0)].x_dim, steps);

    // std::vector<double> vec(x_old.size());
    // Eigen::Map<arma::mat>(vec.data(), x_old.n_rows, x_old.n_cols) =x_old; //

    arma::mat u_old = arma::mat(old.x_mod[(int)old.q_0[0](n, 0)].u_dim, 1);
    u_old = old.x_data.u_k;
    arma::mat d_old = arma::mat(old.x_mod[(int)old.q_0[0](n, 0)].d_dim, 1);
    d_old = old.x_data.d_k;

    if (old.Q[0] == 1) {
      x_new = old.x_mod[0].updateLTIst(old.x_mod[0], x_old, u_old, d_old);
    } else {

      double equal =
          arma::accu(old.x_mod[(int)old.q_0[0](n, 0)].sigma ==
                     arma::zeros<arma::mat>(
                         old.x_mod[(int)old.q_0[0](n, 0)].sigma.n_rows,
                         old.x_mod[(int)old.q_0[0](n, 0)].sigma.n_cols));
      if (equal > 0) {
        for (int j = 0; j < steps; j++) {
          int q_0 = (int)old.q_0[0](n, j);
          if (old.sigmoid) {
            // Generalise: assumes for now that only have pairs of discrete
            // modes
            old.p_k[0](0, 0) = sigmoidCompute(x_old(0, 0), 100, 19.5);
            old.p_k[0](0, 1) = 1 - old.p_k[0](0, 0);
            old.p_k[0](1, 1) = 1 - old.p_k[0](0, 0);
            old.p_k[0](1, 0) = old.p_k[0](1, 1);
            std::cout << "Trans: " << old.p_k[0] << std::endl;
          } else {
            if (n > 0) {
              old.p_k[0] = old.p_k[0] * old.Tq[0];
            }
          }
          std::vector<int> q = t_q(old, q_0, 0);
          int q_new = q[1];
          if (old.x_mod[(int)old.q_0[0](n, j)].d_dim == 0) {
            x_new.col(j) = old.x_mod[q_new].updateLTI(
                old.x_mod[(int)old.q_0[0](n, j)], x_old.col(j), u_old);
          } else {
            x_new.col(j) = old.x_mod[q_new].updateLTIad(
                old.x_mod[(int)old.q_0[0](n, j)], x_old.col(j), u_old, d_old);
          }
          // Append result to Q in old object (columns)
          old.q_0[0](n + 1, j) = q_new;
        }
      } else {
        // Get next state by updating p_k
        // Assumes continuous variables are affected by white noise
        // described using gaussian distribution with mean x[k] = x_old,
        // variance sigma
        // Compute conditional distribution for each mode
        for (int j = 0; j < steps; j++) {
          std::vector<int> q_0;
          q_0.push_back(old.q_0[0](n, j));
          q_0.push_back(old.q_0[1](n, j));
          std::cout << "q_0: " << q_0[0] << ", x_old: " << x_old << " ,j: " << j
                    << std::endl;
          std::cout << "q_0: " << q_0[1] << ", x_old: " << x_old << " ,j: " << j
                    << std::endl;

          if (old.sigmoid) {

            // TODO:Generalise
            old.p_k[0](0, 0) = sigmoidCompute(
                x_old(0, 0), 10, 19.5); //*sigmoidCompute(x_old(1,0),1,21.25);
            old.p_k[0](0, 1) = 1 - old.p_k[0](0, 0);
            old.p_k[0](1, 1) = 1 - old.p_k[0](0, 0);
            old.p_k[0](1, 0) = old.p_k[0](0, 0);
            old.p_k[1](0, 0) = sigmoidCompute(
                x_old(1, 0), 10, 21.25); //*sigmoidCompute(x_old(0,0),1,19.5);
            old.p_k[1](0, 1) = 1 - old.p_k[1](0, 0);
            old.p_k[1](1, 1) = 1 - old.p_k[1](0, 0);
            old.p_k[1](1, 0) = old.p_k[1](0, 0);
            std::cout << "Trans: " << old.p_k[0] << std::endl;
            std::cout << "Trans: " << old.p_k[1] << std::endl;
          } else {
            computeConditional(old, x_old, j, q_0[0]);
          }

          // Sample from conditional distribution to get q_new
          std::vector<int> q0 = t_q(old, q_0[0], 0);
          std::vector<int> q1 = t_q(old, q_0[1], 1);
          std::vector<int> q_new = {q0[1], q1[1]};
          // Append result to Q in old object (columns)
          old.q_0[0](n + 1, j) = q_new[0];
          old.q_0[1](n + 1, j) = q_new[1];

          // Assuming no reset kernels
          arma::mat x_up = arma::mat(1, 1);

          if (old.x_mod[(int)old.q_0[0](n, j)].d_dim == 0) {
            for (int k = 0; k < old.x_mod[q_new[0]].x_dim; k++) {

              x_up = old.x_mod[q_new[k]].updateLTI(
                  old.x_mod[(int)old.q_0[k](n, j)].A.row(k),
                  old.x_mod[(int)old.q_0[k](n, j)].Q.row(k), x_old.col(j));
              x_new(k, j) =
                  getSampleNormal(x_up(0, 0), old.x_mod[q_new[k]].sigma(k, 0));
            }
          } else {
            for (int k = 0; k < old.x_mod[q_new[0]].x_dim; k++) {
              x_up = old.x_mod[q_new[k]].updateLTIad(
                  old.x_mod[(int)old.q_0[k](n, j)].A.row(k),
                  old.x_mod[(int)old.q_0[k](n, j)].B.row(k),
                  old.x_mod[(int)old.q_0[k](n, j)].F.row(k),
                  old.x_mod[(int)old.q_0[k](n, j)].Q.row(k), x_old.col(j),
                  u_old.row(k), d_old.row(k));
              x_new(k, j) =
                  getSampleNormal(x_up(0, 0), old.x_mod[q_new[k]].sigma(k, 0));
            }
          }
        }
      }
      // Append result to X in old object (3rd dimension)
      old.x_data.X.push_back(x_new);
    }
  }
  void computeConditional(shs_t &old, arma::mat x_old, int index, int q_old) {
    double min = -1, max = 1;
    double val, err, xmin[1] = {}, xmax[1] = {};
    std::vector<double> v = {};
    for (int i = 0; i < 2 * old.x_mod[q_old].x_dim; i++) {
      if (i < old.x_mod[q_old].x_dim) {
        xmin[i] = min;
        xmax[i] = max;
        v.push_back(x_old(i, index)); //
        std::cout << "v " << v[i] << std::endl;
      } else {
        // TODO: Case when sigma is correlated
        std::cout << "i-old.x_mod[q_old].x_dim: " << old.x_mod[q_old].x_dim
                  << std::endl;
        std::cout << "q_old " << q_old << std::endl;

        v.push_back(old.x_mod[q_old].sigma(i - old.x_mod[q_old].x_dim, 0));
        std::cout << "v " << v[i] << std::endl;
      }
    }
    val = 1; // hcubature(1, f_Gauss, &v,old.x_mod[n].x_dim, xmin,xmax,
             // 0,0,1e-12, ERROR_INDIVIDUAL, &val,&err);
    // std::cout << "Integral: " << val << std::endl;
    // Update p_k
    if (n == 0) {
      old.p_k[0] = old.Tq[0];
    } else {
      old.p_k[0] = old.p_k[0] * val * old.Tq[0];
    }
    // std::cout << "New kernel: " << old.p_k[0] << std::endl;
  }
  void run(shs_t &old, int N, int steps) {
    // Start simulation timers
    clock_t begin, end;
    begin = clock();
    double time = 0;
    end = clock();

    // For deterministic case perform 1 run
    // For stochastic version perform Monte Carlo + compute mean
    int i = 0;
    int x_dim =old.x_mod[0].x_dim ;
    arma::mat y = arma::zeros<arma::mat>(N * x_dim, steps);
    arma::mat modes = arma::zeros<arma::mat>(N, steps);
    old.p_k = old.Tq;
    int count = 0;
    while (i < N) {
      old.step(old, i, steps);
      if (old.x_mod[0].u_dim > 0) {
        if ((unsigned)old.x_data.U.n_rows == (unsigned)N) {
          old.x_data.u_k = old.x_data.U.row(i);
        }
      }
      if (old.x_mod[0].d_dim > 0) {
        if ((unsigned)old.x_data.D.n_rows == (unsigned)N) {
          old.x_data.d_k = old.x_data.D.row(i);
        }
      }
      arma::mat tempX = old.x_data.X[i];
      if(old.x_mod[0].x_dim == 1) {
        y.row(count) = tempX;
      }
      else {
        y.rows(count, count + 1) = tempX;
      }
      modes.row(i) = old.q_0[0].row(i);
      count += old.x_mod[0].x_dim ;
      i++;
    }

    time = (double)(end - begin) / CLOCKS_PER_SEC;
    std::ostringstream oss;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    oss << std::put_time(&tm, "%d-%m-%Y-%H-%M-%S");
    auto str = oss.str();

    std::cout << std::endl;
    std::cout << "--------------------------------------" <<std::endl;
    std::cout << " Simulation time                      " << std::endl;
    std::cout << "--------------------------------------" <<std::endl;
    std::cout << " " << time << std::endl;
    std::cout << "--------------------------------------" <<std::endl;
    std::cout << std::endl;

    // Option to export to file results
    std::ofstream myfile;
    std::string exportOpt;
    std::string str0("y");
    std::string str1("yes");
    std::cout << "Would you like to store simulation results [y- yes, n - no] " << std::endl;
    std::cin >> exportOpt;
    if ((exportOpt.compare(str0) == 0) || (exportOpt.compare(str1) == 0)) {
      // check if results folder exists:
      if(checkFolderExists("../results") == -1) {
        if(mkdir("../results", 0777) == -1) {
          std::cerr << "Error cannot create results directory: " <<std::strerror(errno) <<std::endl;
          exit(0);
         }
      }
      // Store results in file
      std::string f_name = "../results/Simulationtime_" + str + ".txt";
      myfile.open(f_name);
      myfile << time <<std::endl;
      myfile.close();
      std::string y_name = "../results/y_" + str + ".txt";
      y.save(y_name, arma::raw_ascii);
      arma::mat q = old.q_0[0];
      std::string q_name = "../results/modes_" + str + ".txt";
      q.save(q_name, arma::raw_ascii);
    }
  }
  void obtainTqfromMat(const char *fn, shs_t &init) {
    // Reading model file input in .arma::mat format
    // and storing into ssmodel class
    mat_t *matf;
    matvar_t *matvar, *contents;
    // Read .arma::mat file
    try {
      matf = Mat_Open(fn, MAT_ACC_RDONLY);
      if (matf) // if successful in reading file
      {
        // read each variable within file and populate
        // state space model based on variable name
        contents = Mat_VarRead(matf, "Tq");
        if (contents == NULL) {
          std::cout << "Variable Tq not found in file" << std::endl;
          std::cout << "Number of modes set to 1" << std::endl;
          init.Q[0] = 1;
        } else {
          init.populateTq(*contents);
          // Mat_VarFree(matvar);
          matvar = NULL;
          contents = NULL;
        }
      } else // unsuccessfull in opening file
      {
        throw "Error opening mat file";
      }
      Mat_Close(matf);
    } catch (const char *msg) {
      std::cout << msg << std::endl;
      exit(0);
    }
  }

private:
  int checkData() {
    int error = 0;
    if ((unsigned)this->x_mod[0].x_dim != (unsigned)this->x_data.X[0].n_rows) {
      error = 1;
    }
    if ((unsigned)this->x_mod[0].u_dim != (unsigned)this->x_data.U.n_rows &&
        (unsigned)this->x_mod[0].u_dim > 0) {
      error = 2;
    }
    if ((unsigned)this->x_mod[0].d_dim != (unsigned)this->x_data.D.n_rows &&
        (unsigned)this->x_mod[0].d_dim > 0) {
      error = 3;
    }
    return error;
  }
  void populateTq(matvar_t &content) {
    ssmodels_t container;

    // Tq can be of two version
    // Cells containing strings for guards or
    // Numeric containing probabilities
    if (content.data != NULL) {
      if (content.data_type == MAT_T_DOUBLE) {
        std::string str;
        size_t stride = Mat_SizeOf(content.data_type);
        char *data = (char *)content.data;
        unsigned i, j = 0;
        for (i = 0; i < content.dims[0]; i++) {
          for (j = 0; j < content.dims[1]; j++) {
            size_t idx = content.dims[0] * j + i;
            void *t = data + idx * stride;
            char substr[100];
            sprintf(substr, "%g",
                    *(double *)t); // Assumes values are of type double
            str.append(substr);
            str.append(" ");
          }
          str.append(";");
        }
        std::vector<std::string> x = splitStr(str, ';');
        int numEl = x.size();
        std::cout << numEl << std::endl;
        std::cout << "modes: " << numEl << std::endl;
        this->Q[0] = numEl;

        // Check stochasticity of kernel
        arma::mat tq = strtodMatrix(x);
        tq = checkStochasticity(tq.t());
        this->Tq[0] = tq;
      } else {
        std::cout << "Incorrect Tq format" << std::endl;
      }
    } else {
      std::cout << "Tq field not input in arma::mat file" << std::endl;
    }
  }
};

#endif
