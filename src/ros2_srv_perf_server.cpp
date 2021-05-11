// Copyright 2014 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cinttypes>
#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "ros2_srv_perf/srv/check_time.hpp"

namespace ros2_srv_perf
{

class ServerNode : public rclcpp::Node
{
public:
  explicit ServerNode(const rclcpp::NodeOptions & options)
  : Node("ros2_srv_perf_server", options)
  {
    auto handle_add_two_ints =
      [this](const std::shared_ptr<rmw_request_id_t> request_header,
        const std::shared_ptr<ros2_srv_perf::srv::CheckTime::Request> request,
        std::shared_ptr<ros2_srv_perf::srv::CheckTime::Response> response) -> void
      {
        (void)request_header;
        RCLCPP_INFO(
          this->get_logger(), "Incoming request (%lu)\na: %" PRId64 " b: %" PRId64,
          request->sent_time_us, request->a, request->b);
        response->sum = request->a + request->b;
        response->result_time_us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      };
    // Create a service that will use the callback function to handle requests.
    srv_ = create_service<ros2_srv_perf::srv::CheckTime>("check_time", handle_add_two_ints);
  }

private:
  rclcpp::Service<ros2_srv_perf::srv::CheckTime>::SharedPtr srv_;
};

}  // namespace ros2_srv_perf

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<ros2_srv_perf::ServerNode>(rclcpp::NodeOptions());

  rclcpp::spin(node);

  rclcpp::shutdown();

  return 0;
}
