package com.stm.ble_mcsdk.activity.main

import android.Manifest
import android.annotation.SuppressLint
import android.bluetooth.BluetoothAdapter
import android.content.Intent
import android.os.Build
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.TypedValue
import android.view.Menu
import android.view.MenuItem
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.databinding.DataBindingUtil
import com.stm.ble_mcsdk.MCSDKViewModel
import com.stm.ble_mcsdk.R
import com.stm.ble_mcsdk.ble.BLEManager
import com.stm.ble_mcsdk.ble.BLEManager.bAdapter
import com.stm.ble_mcsdk.ble.ENABLE_BLUETOOTH_REQUEST_CODE
import com.stm.ble_mcsdk.databinding.ActivityMainBinding
import com.stm.ble_mcsdk.fragment.log.LogFragment
import com.stm.ble_mcsdk.fragment.reset.ResetFragment
import com.stm.ble_mcsdk.fragment.scan.ScanFragment
import com.stm.ble_mcsdk.log.LogManager
import android.content.Context
import android.view.KeyEvent
import android.view.View
import android.view.inputmethod.EditorInfo
import android.view.inputmethod.InputMethodManager
import android.widget.EditText
import androidx.lifecycle.MutableLiveData
import kotlinx.coroutines.launch

@SuppressLint("SetTextI18n", "MissingPermission")
class MainActivity : AppCompatActivity(), MainInterface {

    private lateinit var binding: ActivityMainBinding
    private val viewModel: MCSDKViewModel by viewModels()
    private var searchItem: MenuItem? = null

    private val scanFragment = ScanFragment()
    private val logFragment = LogFragment()

    private val sliderHandler = Handler(Looper.getMainLooper())
    private val sliderThrottleIntervalms = 25L
    private var canSendSlider1 = true
    private var canSendSlider2 = true
    private var canSendSlider3 = true
    private var canSendSlider4 = true
    private var canSendSlider5 = true
    private var pendingSlider1Value: Int? = null
    private var pendingSlider2Value: Int? = null
    private var pendingSlider3Value: Int? = null
    private var pendingSlider4Value: Int? = null
    private var pendingSlider5Value: Int? = null


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = DataBindingUtil.setContentView(this, R.layout.activity_main)
        binding.lifecycleOwner = this
        binding.main = this
        binding.vm = viewModel
        viewModel.mainInterface = this
        BLEManager.mainInterface = this
        BLEManager.viewModel = viewModel
        LogManager.logInterface = logFragment
        setSupportActionBar(binding.toolbar)

        toggleFunctionality(false)
        updateStatus("Idle")
        //slider()
        setupSlidersWithThrottling()
        setupEditTextInteractionListeners()
    }

    override fun onResume() {
        super.onResume()

        if (!bAdapter.isEnabled) {
            promptEnableBluetooth()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        toggleFunctionality(false)
        BLEManager.bGatt?.disconnect()
    }

    /** Permission & Bluetooth Requests */

    // Prompt to Enable BT
    override fun promptEnableBluetooth() {
        if(!bAdapter.isEnabled){
            val enableBtIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            ActivityCompat.startActivityForResult(
                this, enableBtIntent, ENABLE_BLUETOOTH_REQUEST_CODE, null
            )
        }
    }

    // Request Runtime Permissions (Based on Android Version)
    private fun requestPermissions() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            requestMultiplePermissions.launch(arrayOf(
                Manifest.permission.ACCESS_FINE_LOCATION,
                Manifest.permission.BLUETOOTH_SCAN,
                Manifest.permission.BLUETOOTH_CONNECT
            ))
        } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            requestMultiplePermissions.launch(arrayOf(
                Manifest.permission.ACCESS_FINE_LOCATION
            ))
        }
    }

    private val requestMultiplePermissions =
        registerForActivityResult(ActivityResultContracts.RequestMultiplePermissions()) {}

    // Rerequest Permissions if Not Given by User (Limit 2)
    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)

        if (BLEManager.hasPermissions(this)) {
            scanFragment.show(supportFragmentManager, "scanFragment")
        } else {
            requestPermissions()
        }
    }

    /** Toolbar Menu */

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        menuInflater.inflate(R.menu.toolbar, menu)
        searchItem = menu.findItem(R.id.searchItem)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        when(item.itemId) {
            R.id.searchItem -> {
                if (BLEManager.isConnected) {
                    // Toggle Views & Set Speed Before Disconnecting
                    toggleFunctionality(false)
                    BLEManager.disconnect()
                } else {
                    if (!BLEManager.hasPermissions(this)) {
                        requestPermissions()
                        return false
                    }

                    scanFragment.show(supportFragmentManager, "scanFragment")
                }
            }
            R.id.logItem -> {
                logFragment.show(supportFragmentManager, "logFragment")
            }
        }

        return false
    }

    /** DMX Functionality */

    // DMX Image Clicked
    fun onDMXClick() {
        with (viewModel) {
            isDMXOn = !isDMXOn

            if (isDMXOn) {
                BLEManager.scope.launch {
                    // Turn On DMX
                    writeCharacteristic("0001")
                    LogManager.addLog("Write DMX", "ON")

                }
                setDMXColor(R.attr.colorPrimary)
            } else {
                BLEManager.scope.launch {
                    // Turn Off DMX
                    writeCharacteristic("0000")
                    LogManager.addLog("Write DMX", "OFF")
                }
                setDMXColor(null)
            }

        }
    }


    @SuppressLint("RestrictedApi")
    private fun setupSlidersWithThrottling() {
        with(binding) {
            // Slider 1
            valueSlider.addOnChangeListener { _, value, fromUser ->
                if (fromUser) {
                    val currentValue = value.toInt()
                    pendingSlider1Value = currentValue
                    if (canSendSlider1) {
                        canSendSlider1 = false
                        viewModel.setValueMessage(currentValue, viewModel.DMXch1.value ?: 1)
                        viewModel.DMXval1.value = currentValue

                        sliderHandler.postDelayed({
                            canSendSlider1 = true
                            pendingSlider1Value?.let {
                                if (it != currentValue) { 
                                    viewModel.setValueMessage(it, viewModel.DMXch1.value ?: 1)
                                    viewModel.DMXval1.value = it
                                }
                                pendingSlider1Value = null
                            }
                        }, sliderThrottleIntervalms)
                    }
                }
            }

            // Slider 2
            valueSlider2.addOnChangeListener { _, value, fromUser ->
                if (fromUser) {
                    val currentValue = value.toInt()
                    pendingSlider2Value = currentValue
                    if (canSendSlider2) {
                        canSendSlider2 = false
                        viewModel.setValueMessage(currentValue, viewModel.DMXch2.value ?: 2)
                        viewModel.DMXval2.value = currentValue

                        sliderHandler.postDelayed({
                            canSendSlider2 = true
                            pendingSlider2Value?.let {
                                if (it != currentValue) {
                                    viewModel.setValueMessage(it, viewModel.DMXch2.value ?: 2)
                                    viewModel.DMXval2.value = it
                                }
                                pendingSlider2Value = null
                            }
                        }, sliderThrottleIntervalms)
                    }
                }
            }

            valueSlider3.addOnChangeListener { _, value, fromUser ->
                if (fromUser) {
                    val currentValue = value.toInt()
                    pendingSlider3Value = currentValue
                    if (canSendSlider3) {
                        canSendSlider3 = false
                        viewModel.setValueMessage(currentValue, viewModel.DMXch3.value ?: 3)
                        viewModel.DMXval3.value = currentValue

                        sliderHandler.postDelayed({
                            canSendSlider3 = true
                            pendingSlider3Value?.let {
                                if (it != currentValue) {
                                    viewModel.setValueMessage(it, viewModel.DMXch3.value ?: 3)
                                    viewModel.DMXval3.value = it
                                }
                                pendingSlider3Value = null
                            }
                        }, sliderThrottleIntervalms)
                    }
                }
            }

            valueSlider4.addOnChangeListener { _, value, fromUser ->
                if (fromUser) {
                    val currentValue = value.toInt()
                    pendingSlider4Value = currentValue
                    if (canSendSlider4) {
                        canSendSlider4 = false
                        viewModel.setValueMessage(currentValue, viewModel.DMXch4.value ?: 4)
                        viewModel.DMXval4.value = currentValue

                        sliderHandler.postDelayed({
                            canSendSlider4 = true
                            pendingSlider4Value?.let {
                                if (it != currentValue) {
                                    viewModel.setValueMessage(it, viewModel.DMXch4.value ?: 4)
                                    viewModel.DMXval4.value = it
                                }
                                pendingSlider4Value = null
                            }
                        }, sliderThrottleIntervalms)
                    }
                }
            }

            valueSlider5.addOnChangeListener { _, value, fromUser ->
                if (fromUser) {
                    val currentValue = value.toInt()
                    pendingSlider5Value = currentValue
                    if (canSendSlider5) {
                        canSendSlider5 = false
                        viewModel.setValueMessage(currentValue, viewModel.DMXch5.value ?: 5)
                        viewModel.DMXval5.value = currentValue

                        sliderHandler.postDelayed({
                            canSendSlider5 = true
                            pendingSlider5Value?.let {
                                if (it != currentValue) {
                                    viewModel.setValueMessage(it, viewModel.DMXch5.value ?: 5)
                                    viewModel.DMXval5.value = it
                                }
                                pendingSlider5Value = null
                            }
                        }, sliderThrottleIntervalms)
                    }
                }
            }
        }
    }

    private fun setupEditTextInteractionListeners() {
        // IDs de tus EditText: Ch, Ch2, Ch3, etc.
        setupListenerForChannelEditText(binding.Ch1, viewModel.DMXch1, 1)
        setupListenerForChannelEditText(binding.Ch2, viewModel.DMXch2, 2)
        setupListenerForChannelEditText(binding.Ch3, viewModel.DMXch3, 3)
        setupListenerForChannelEditText(binding.Ch4, viewModel.DMXch4, 4)
        setupListenerForChannelEditText(binding.Ch5, viewModel.DMXch5, 5)
    }

    private fun setupListenerForChannelEditText(editText: EditText, dmxChannelLiveData: MutableLiveData<Int>, defaultChannelValue: Int) {
        val processEditTextValue = {
            val text = editText.text.toString()
            var channelToSet: Int? = null

            if (text.isEmpty()) {
                val hintText = editText.hint?.toString()
                channelToSet = hintText?.toIntOrNull() ?: defaultChannelValue
                editText.setText(channelToSet.toString())
            } else {
                text.toIntOrNull()?.let { enteredValue ->
                    channelToSet = when {
                        enteredValue in 1..512 -> enteredValue
                        enteredValue <= 0 -> 1
                        else -> 512
                    }
                } ?: run {
                    channelToSet = defaultChannelValue
                }
            }

            channelToSet?.let { finalValue ->
                if (dmxChannelLiveData.value != finalValue) {
                    dmxChannelLiveData.value = finalValue
                    val resourceIdName = try {
                        editText.resources.getResourceEntryName(editText.id)
                    } catch (e: Exception) {
                        "UnknownEditTextId"
                    }
                    LogManager.addLog(resourceIdName, "Channel set to $finalValue")
                }
            }
        }
        editText.setOnEditorActionListener { textView, actionId, keyEvent ->
            if (actionId == EditorInfo.IME_ACTION_DONE ||
                (keyEvent != null && keyEvent.keyCode == KeyEvent.KEYCODE_ENTER && keyEvent.action == KeyEvent.ACTION_DOWN)
            ) {
                processEditTextValue()
                val imm = getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
                imm.hideSoftInputFromWindow(textView.windowToken, 0)
                textView.clearFocus()
                return@setOnEditorActionListener true
            }
            false
        }
        editText.onFocusChangeListener = View.OnFocusChangeListener { _, hasFocus ->
            if (!hasFocus) {
                processEditTextValue()
            }
        }
    }

    // ST Logo Clicked
    fun onSTLogoClick() {
        if (BLEManager.isConnected) {
            ResetFragment().show(supportFragmentManager, "resetFragment")
        }
    }

    /** Helper Functions */

    // Toggle Functionality based on BLE Connection
    override fun toggleFunctionality(connected: Boolean) {
        runOnUiThread {
            with(binding) {
                DMXImage.isEnabled = connected

                if (connected) {
                    // Connected to BLE Device
                    searchItem?.setIcon(R.drawable.ic_cancel)
                } else {
                    // Not Connected to BLE Device
                    searchItem?.setIcon(R.drawable.ic_search)
                    setDMXColor(null)
                    viewModel.isDMXOn = false
                }
            }
        }
    }


    // Set DMX Image Color
    private fun setDMXColor(color: Int?) {
        if (color == null) {
            // Reset Color
            binding.DMXImage.setImageResource(R.drawable.ic_launcher_foreground)
        } else {
            // Change Color to Theme Color
            val value = TypedValue()
            theme.resolveAttribute (color, value, true)
            binding.DMXImage.drawable.setTint(
                value.data
            )
        }
    }

    // Update BLE Status Text
    override fun updateStatus(status: String) {
        runOnUiThread {
            binding.statusText.text = status
        }
    }

    // Show Toast Message
    override fun showToast(message: String) {
        runOnUiThread {
            Toast.makeText(this, message, Toast.LENGTH_SHORT).show()
        }
    }

}