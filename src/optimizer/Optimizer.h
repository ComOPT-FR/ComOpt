#include <Eigen/Core>
class Optimizer {
public:
    virtual Eigen::VectorXd optimize() = 0;
};
