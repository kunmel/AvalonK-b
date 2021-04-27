# Flow

:one: Submit work order

User
generic_client.py
tx_committer.py/cc_invoke(order/workOrderSubmit)
order链码生成事件workOrderSubmitted
fabirc_work_order.py/get_work_order_submitted_event_handler(handler_func)   :left_right_arrow:   监听work order提交事件​
fabric_connector.py/start_wo_submitted_event_listener 
base_connector.py/sync_work_orders    :left_right_arrow:  将区块链以及Avalon上的work order同步
base_connector.py/work_order_submitted_event_handler  :left_right_arrow:   将work order提交到listener，得到结果并将结果存储到区块链

:two: Use singleton.toml

base_enclave_manger.py/EnclaveManager  :left_right_arrow:  此类使用了config文件的config("WorkerConfig")["worker_id"]
singleton_enclave_manager.py  :left_right_arrow:  使用了EnclaveManager类并传入了singleton.toml