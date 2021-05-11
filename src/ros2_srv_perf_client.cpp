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

#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "ros2_srv_perf/srv/check_time.hpp"

using namespace std::chrono_literals;

ros2_srv_perf::srv::CheckTime::Response::SharedPtr send_request(
  rclcpp::Node::SharedPtr node,
  rclcpp::Client<ros2_srv_perf::srv::CheckTime>::SharedPtr client,
  ros2_srv_perf::srv::CheckTime::Request::SharedPtr request)
{
  auto result = client->async_send_request(request);
  // Wait for the result.
  if (rclcpp::spin_until_future_complete(node, result) ==
    rclcpp::FutureReturnCode::SUCCESS)
  {
    return result.get();
  } else {
    return NULL;
  }
}

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("ros2_srv_perf_client");

  auto client = node->create_client<ros2_srv_perf::srv::CheckTime>("check_time");

  while (!client->wait_for_service(1s)) {
    if (!rclcpp::ok()) {
      RCLCPP_ERROR(node->get_logger(), "Interrupted while waiting for the service. Exiting.");
      return 0;
    }
    RCLCPP_INFO(node->get_logger(), "service not available, waiting again...");
  }

  auto request = std::make_shared<ros2_srv_perf::srv::CheckTime::Request>();
  request->a = 2;
  request->b = 3;
  request->sent_time_us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

  auto start_time_us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  auto result = send_request(node, client, request);
  auto end_time_us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  if (result) {
    RCLCPP_INFO_STREAM(node->get_logger(), "Result of add_two_ints: " << result->sum << ", took " << end_time_us - start_time_us << " total us, " << result->result_time_us - request->sent_time_us << " us to compute the result");
  } else {
    RCLCPP_ERROR(node->get_logger(), "Interrupted while waiting for response. Exiting.");
  }

  rclcpp::shutdown();
  return 0;
}
