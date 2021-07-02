#pragma once

#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "drake/geometry/optimization/convex_set.h"

namespace drake {
namespace geometry {
namespace optimization {

/** A convex set that contains exactly one element.  Also known as a
singleton or unit set. */
class Point final : public ConvexSet {
 public:
  DRAKE_DEFAULT_COPY_AND_MOVE_AND_ASSIGN(Point)

  /** Constructs a Point. */
  explicit Point(const Eigen::Ref<const Eigen::VectorXd>& x);

  /** Constructs a Point from a SceneGraph geometry and pose in
  the @p expressed_in frame, obtained via the QueryObject. If @p expressed_in
  frame is std::nullopt, then it will be expressed in the world frame.

  @throws std::exception if geometry_id does not correspond to a Sphere or if
  the Sphere has radius greater than @p maximum_allowable_radius. */
  Point(const QueryObject<double>& query_object, GeometryId geometry_id,
        std::optional<FrameId> expressed_in = std::nullopt,
        double maximum_allowable_radius = 0.0);

  ~Point() final;

  /** Returns true if the point is within @p tol of x(), under the L∞-norm. */
  using ConvexSet::PointInSet;

  /** Retrieves the point. */
  const Eigen::VectorXd& x() const { return x_; }

 private:
  bool DoPointInSet(const Eigen::Ref<const Eigen::VectorXd>& x,
                    double tol) const final;

  void DoAddPointInSetConstraint(
      solvers::MathematicalProgram*,
      const Eigen::Ref<const solvers::VectorXDecisionVariable>&) const final;

  std::vector<solvers::Binding<solvers::Constraint>>
  DoAddPointInNonnegativeScalingConstraints(
      solvers::MathematicalProgram* prog,
      const Eigen::Ref<const solvers::VectorXDecisionVariable>& x,
      const symbolic::Variable& t) const final;

  std::pair<std::unique_ptr<Shape>, math::RigidTransformd> DoToShapeWithPose()
      const final;

  // Implement support shapes for the ShapeReifier interface.
  using ShapeReifier::ImplementGeometry;
  void ImplementGeometry(const Sphere& sphere, void* data) final;

  Eigen::VectorXd x_;
};

}  // namespace optimization
}  // namespace geometry
}  // namespace drake
