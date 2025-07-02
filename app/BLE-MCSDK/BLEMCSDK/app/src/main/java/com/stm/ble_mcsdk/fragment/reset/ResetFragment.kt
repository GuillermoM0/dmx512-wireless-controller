package com.stm.ble_mcsdk.fragment.reset

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.WindowManager
import androidx.databinding.DataBindingUtil
import androidx.fragment.app.DialogFragment
import androidx.fragment.app.activityViewModels
import com.stm.ble_mcsdk.MCSDKViewModel
import com.stm.ble_mcsdk.R
import com.stm.ble_mcsdk.ble.BLEManager
import com.stm.ble_mcsdk.databinding.FragmentResetBinding
import com.stm.ble_mcsdk.log.LogManager
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch

class ResetFragment: DialogFragment() {

    private lateinit var binding: FragmentResetBinding
    private val viewModel: MCSDKViewModel by activityViewModels()

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        binding = DataBindingUtil.inflate(
            inflater,
            R.layout.fragment_reset,
            container,
            false
        )

        binding.fragment = this

        return binding.root
    }

    override fun onResume() {
        super.onResume()

        // Set Fragment Dimensions
        dialog?.window?.setLayout(
            WindowManager.LayoutParams.MATCH_PARENT,
            WindowManager.LayoutParams.WRAP_CONTENT
        )
    }

    /** Buttons */
    // DMX Button Clicked
    fun resetDMX() {
        BLEManager.scope.launch {
            // Reset DMX
            viewModel.writeCharacteristic("FF")
            viewModel.DMXval1.postValue(0)
            viewModel.DMXval2.postValue(0)
            viewModel.DMXval3.postValue(0)
            viewModel.DMXval4.postValue(0)
            viewModel.DMXval5.postValue(0)
            viewModel.DMXch1.postValue(1)
            viewModel.DMXch2.postValue(2)
            viewModel.DMXch3.postValue(3)
            viewModel.DMXch4.postValue(4)
            viewModel.DMXch5.postValue(5)
            LogManager.addLog("Reset DMX", "All 0")
        }
        dismissFragment()
    }

    // Cancel Button Clicked
    fun dismissFragment() {
        dismiss()
    }

}